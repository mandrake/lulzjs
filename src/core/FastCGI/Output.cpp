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

#include "Output.h"

void
Output_initialize (JSContext* cx)
{

}

JSBool
Output_write (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    JS_BeginRequest(cx);
    JS_EnterLocalRootScope(cx);

    LCGIData* data = (LCGIData*) JS_GetContextPrivate(cx);

    char*        separator = " ";
    char*        end       = "\n";
    FCGX_Stream* fp        = data->cgi->out;
    jsval        property;
    JSObject*    options;

    if (argc > 1 && JS_TypeOfValue(cx, argv[argc-1]) == JSTYPE_OBJECT) {
        JS_ValueToObject(cx, argv[argc-1], &options);
        argc--;

        JS_GetProperty(cx, options, "separator", &property);
        if (JSVAL_IS_VOID(property) || JSVAL_IS_NULL(property)) {
            JS_GetProperty(cx, options, "sep", &property);
        }

        if (JSVAL_IS_STRING(property)) {
            separator = JS_GetStringBytes(JS_ValueToString(cx, property));
        }

        JS_GetProperty(cx, options, "end", &property);
        if (JSVAL_IS_STRING(property)) {
            end = JS_GetStringBytes(JS_ValueToString(cx, property));
        }
    }

    uintN i;
    for (i = 0; i < argc; i++) {
        FCGX_FPrintF(fp, "%s", JS_GetStringBytes(JS_ValueToString(cx, argv[i])));

        if (i != argc-1) {
            FCGX_FPrintF(fp, "%s", separator);
        }
    }
    FCGX_FPrintF(fp, "%s", end);

    JS_LeaveLocalRootScope(cx);
    JS_EndRequest(cx);

    return JS_TRUE;
}


