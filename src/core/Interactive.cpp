/****************************************************************************
* This file is part of lulzJS                                               *
* Copyleft meh. [http://meh.doesntexist.org | meh.ffff@gmail.com]           *
*                                                                           *
* lulzJS is free software: you can redistribute it and/or modify            *
* it under the terms of the GNU General Public License as published by      *
* the Free Software Foundation, either version 3 of the License, or         *
* (at your option) any later version.                                       *
*                                                                           *
* lulzJS is distributed in the hope that it will be useful.                 *
* but WITHOUT ANY WARRANTY; without even the implied warranty o.            *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See th.             *
* GNU General Public License for more details.                              *
*                                                                           *
* You should have received a copy of the GNU General Public License         *
* along with lulzJS.  If not, see <http://www.gnu.org/licenses/>.           *
****************************************************************************/

#include "Interactive.h"

void
Interactive (JSContext* cx, JSObject* global)
{
    JSBool exit = JS_FALSE;

    jsval     result;
    JSString* strResult;

    char* whole;
    char* line;
    int lineNumber = 0;

    int  startline;
    int  lineno;

    std::cout << "lulzJS " << __LJS_VERSION__ << std::endl << std::endl;

    do {
        startline = lineno = 1;
        whole = NULL;

        do {
            line = readline(startline == lineno ? ">>> " : "... ");

            // If it's the first line and nothing has been written or the line
            // is quit exit the interactive mode.
            if (startline == lineno) {
                if (line == NULL || strcmp(line, "quit") == 0) {
                    std::cout << "GTFO" << std::endl;
                    return;
                }
            }

            // Realloc the whole string with length of whole + the length of the inserted line.
            whole = (char*) realloc(whole, (whole==NULL?0:strlen(whole))+strlen(line)*sizeof(char)+1);

            // If it's the first line use strcpy to avoid not cleaned memory bugs if it's not
            // use strcat.
            whole = (lineno == 1) ? strcpy(whole, line) : strcat(whole, line);

            free(line);
            lineno++; lineNumber++;
        } while (!lulzJS::Script::isCompilable(cx, whole));

        if (strlen(whole) == 0) {
            continue;
        }

        add_history(whole);


        JS_BeginRequest(cx);
        JS_EnterLocalRootScope(cx);

        try {
            lulzJS::Script script(cx, whole, "ljs", lineNumber);
            jsval val = script.execute();

            if (!JSVAL_IS_VOID(val)) {
                jsval jsObj; JS_GetProperty(cx, global, "Object", &jsObj);
                JSObject* object = JSVAL_TO_OBJECT(jsObj);

                jsval argv[] = {val};
                JS_CallFunctionName(cx, object, "inspect", 1, argv, &result);

                strResult = JS_ValueToString(cx, result);

                if (strResult) {
                    std::string str = JS_GetStringBytes(strResult);

                    if (str != "undefined") {
                        std::cout << "=> " << JS_GetStringBytes(strResult) << std::endl;
                    }
                }
            }
        }
        catch (std::exception e) {}

        JS_LeaveLocalRootScope(cx);

        JS_ClearPendingException(cx);
        free(whole);

        JS_EndRequest(cx);
    } while (!exit);
}

