/****************************************************************************
* This file is part of lulzJS                                               *
* Copyleft meh.                                                             *
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

#include "Console.h"

JSBool exec (JSContext* cx) { return Console_initialize(cx); }

JSBool
Console_initialize (JSContext* cx)
{
    JS_BeginRequest(cx);

    jsval jsParent;
    JS_GetProperty(cx, JS_GetGlobalObject(cx), "System", &jsParent);
    JSObject* parent = JSVAL_TO_OBJECT(jsParent);

    JSObject* object = JS_DefineObject(
        cx, parent,
        Console_class.name, &Console_class, NULL, 
        JSPROP_PERMANENT|JSPROP_READONLY|JSPROP_ENUMERATE
    );

    if (object) {
        JS_DefineFunctions(cx, object, Console_methods);

        JS_EndRequest(cx);
        return JS_TRUE;
    }

    return JS_FALSE;
}

JSBool
Console_write (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval)
{
    char* string;

    JS_BeginRequest(cx);

    if (argc != 1 || !JS_ConvertArguments(cx, argc, argv, "s", &string)) {
        JS_ReportError(cx, "Not enough parameters.");
        return JS_FALSE;
    }

    printf("%s", string);
    fflush(stdout);

    JS_EndRequest(cx);
    return JS_TRUE;
}

JSBool
Console_error (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval)
{
    char* string;

    JS_BeginRequest(cx);

    if (argc != 1 || !JS_ConvertArguments(cx, argc, argv, "s", &string)) {
        JS_ReportError(cx, "Not enough parameters.");
        return JS_FALSE;
    }

    fprintf(stderr, "%s", string);
    fflush(stderr);

    JS_EndRequest(cx);
    return JS_TRUE;
}

JSBool
Console_readLine (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval)
{
    JS_BeginRequest(cx);
    JS_EnterLocalRootScope(cx);

    char* string  = (char*) JS_malloc(cx, 16*sizeof(char));
    size_t length = 0;
    
    do {
        if ((length+1) % 16) {
            string = (char*) JS_realloc(cx, string, (length+16+1)*sizeof(char));
        }
        
        string[length] = (char) getchar();
    } while (string[(++length)-1] != '\n');
    
    string[length-1] = '\0';
    string = (char*) JS_realloc(cx, string, length*sizeof(char));
    
    *rval = STRING_TO_JSVAL(JS_NewString(cx, string, strlen(string)));

    JS_LeaveLocalRootScope(cx);
    JS_EndRequest(cx);
    
    return JS_TRUE;
}

