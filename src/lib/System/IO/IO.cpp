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

#include "IO.h"

JSBool exec (JSContext* cx) { return IO_initialize(cx); }

JSBool
IO_initialize (JSContext* cx)
{
    JS_BeginRequest(cx);

    jsval jsParent   = JS_EVAL(cx, "System");
    JSObject* parent = JSVAL_TO_OBJECT(jsParent);

    JSObject* object = JS_DefineObject(
        cx, parent,
        IO_class.name, &IO_class, NULL, 
        JSPROP_PERMANENT|JSPROP_READONLY|JSPROP_ENUMERATE);

    if (object) {
        JS_DefineFunctions(cx, object, IO_methods);
        JS_DefineProperties(cx, object, IO_attributes);

        JS_EndRequest(cx);
        return JS_TRUE;
    }

    return JS_FALSE;
}

/*
#undef  MAX
#define MAX(n,m) ((n > m) ? n : m)
#define READ      0
#define WRITE     1
#define EXCEPTION 2
JSBool
IO_select (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval)
{
    fd_set fds[3];
    JSObject* arrays[3];

    JS_BeginRequest(cx);
    JS_EnterLocalRootScope(cx);

    if (argc < 3 || !JS_ConvertArguments(cx, argc, argv, "ooo", &arrays[READ], &arrays[WRITE], &arrays[EXCEPTION])) {
        JS_ReportError(cx, "Not enough parameters.");
        JS_LeaveLocalRootScope(cx);
        JS_EndRequest(cx);
        return JS_FALSE;
    }

    if (!(JS_IsArrayObject(cx, arrays[READ]) && JS_IsArrayObject(cx, arrays[WRITE]) && JS_IsArrayObject(cx, arrays[EXCEPTION]))) {
        JS_ReportError(cx, "You have to pass 3 Arrays.");
        return JS_FALSE;
    }

    FD_ZERO(&fds[READ]); FD_ZERO(&fds[WRITE]); FD_ZERO(&fds[EXCEPTION]);

    jsuint length, i, h;
    jsval  element;
    int32  fd, nfds = 0;

    for (i = 0; i < 3; i++) {
        JS_GetArrayLength(cx, arrays[i], &length);

        for (h = 0; h < length; h++) {
            JS_GetElement(cx, array[i], h, &element);
            JS_ValueToInt32(cx, element, &fd);
            FD_SET(fd, &fds[i]);
        }
    }
}
#undef  MAX
*/
