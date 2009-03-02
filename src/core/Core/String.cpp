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

#include "lulzjs.h"

extern "C" JSBool exec (JSContext* cx);
JSBool String_initialize (JSContext* cx);

JSBool String_toBytes (JSContext* cx, JSObject* object, uintN argc, jsval *argv, jsval* rval);
JSBool String_fromBytes (JSContext* cx, JSObject* object, uintN argc, jsval *argv, jsval* rval);

JSBool exec (JSContext* cx) { return String_initialize(cx); }

JSBool
String_initialize (JSContext* cx)
{
    JS_BeginRequest(cx);
    JS_EnterLocalRootScope(cx);

    JSObject* StringProto = JSVAL_TO_OBJECT(JS_EVAL(cx, "String.prototype"));

    JS_DefineFunction(
        cx, StringProto, 
        "toBytes", String_toBytes,
        0, 0
    );

    JS_DefineFunction(
        cx, StringProto, 
        "fromBytes", String_fromBytes,
        0, 0
    );

    JS_LeaveLocalRootScope(cx);
    JS_EndRequest(cx);
    return JS_TRUE;
}

JSBool
String_toBytes (JSContext* cx, JSObject* object, uintN argc, jsval *argv, jsval* rval)
{
    JSObject* array;

    JS_BeginRequest(cx);
    JS_EnterLocalRootScope(cx);

    JS_LeaveLocalRootScope(cx);
    JS_EndRequest(cx);
    return JS_TRUE;
}

JSBool
String_fromBytes (JSContext* cx, JSObject* object, uintN argc, jsval *argv, jsval* rval)
{
    JSObject* array;

    if (argc < 1) {
        JS_ReportError(cx, "Not enough parameters.");
        return JS_FALSE;
    }

    JS_BeginRequest(cx);
    JS_EnterLocalRootScope(cx);

    JS_LeaveLocalRootScope(cx);
    JS_EndRequest(cx);
    return JS_TRUE;
}

