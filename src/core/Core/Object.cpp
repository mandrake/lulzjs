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
JSBool Object_initialize (JSContext* cx);

JSBool Object_defineProperty (JSContext* cx, JSObject* object, uintN argc, jsval *argv, jsval* rval);
JSBool Object_defineAttributes (JSContext* cx, JSObject* object, uintN argc, jsval *argv, jsval* rval);

JSBool exec (JSContext* cx) { return Object_initialize(cx); }

JSBool
Object_initialize (JSContext* cx)
{
    JS_BeginRequest(cx);
    JS_EnterLocalRootScope(cx);

    JSObject* Object      = JSVAL_TO_OBJECT(JS_EVAL(cx, "Object"));
    JSObject* ObjectProto = JSVAL_TO_OBJECT(JS_EVAL(cx, "Object.prototype"));

    JSObject* Flags = JS_NewObject(cx, NULL, NULL, NULL);
    jsval property  = OBJECT_TO_JSVAL(Flags);
    JS_SetProperty(cx, Object, "Flags", &property);
        property = INT_TO_JSVAL(0);
        JS_SetProperty(cx, Flags, "None", &property);
        property = INT_TO_JSVAL(JSPROP_ENUMERATE);
        JS_SetProperty(cx, Flags, "Default", &property);
        property = INT_TO_JSVAL(JSPROP_ENUMERATE);
        JS_SetProperty(cx, Flags, "Enumerate", &property);
        property = INT_TO_JSVAL(JSPROP_READONLY);
        JS_SetProperty(cx, Flags, "Readonly", &property);
        property = INT_TO_JSVAL(JSPROP_PERMANENT);
        JS_SetProperty(cx, Flags, "Permanent", &property);
        property = INT_TO_JSVAL(JSPROP_SHARED);
        JS_SetProperty(cx, Flags, "Shared", &property);
        property = INT_TO_JSVAL(JSPROP_INDEX);
        JS_SetProperty(cx, Flags, "Index", &property);

    JS_DefineFunction(
        cx, ObjectProto, 
        "__defineProperty__", Object_defineProperty,
        0, 0
    );

    JS_DefineFunction(
        cx, ObjectProto, 
        "__defineAttributes__", Object_defineAttributes,
        0, 0
    );

    JS_LeaveLocalRootScope(cx);
    JS_EndRequest(cx);
    return JS_TRUE;
}

JSBool
Object_defineProperty (JSContext* cx, JSObject* object, uintN argc, jsval *argv, jsval* rval)
{
    char* name   = NULL;
    jsval value  = JSVAL_VOID;
    uint16 attrs = 0;

    if (argc < 2) {
        JS_ReportError(cx, "Not enough parameters.");
        return JS_FALSE;
    }

    JS_BeginRequest(cx);
    JS_EnterLocalRootScope(cx);

    name  = JS_GetStringBytes(JS_ValueToString(cx, argv[0]));
    value = argv[1];

    if (argc == 3) {
        JS_ValueToUint16(cx, argv[2], &attrs);
    }

    JS_DefineProperty(cx, object, name, value, NULL, NULL, attrs);

    JS_LeaveLocalRootScope(cx);
    JS_EndRequest(cx);
    return JS_TRUE;
}

JSBool
Object_defineAttributes (JSContext* cx, JSObject* object, uintN argc, jsval *argv, jsval* rval)
{
    char*     name   = NULL;
    JSObject* value;
    uint16    attrs = 0;

    if (argc < 2) {
        JS_ReportError(cx, "Not enough parameters.");
        return JS_FALSE;
    }

    JS_BeginRequest(cx);
    JS_EnterLocalRootScope(cx);

    name = JS_GetStringBytes(JS_ValueToString(cx, argv[0]));
    JS_ValueToObject(cx, argv[1], &value);

    JSObject* getter;
    JSObject* setter;

    if (argc == 3) {
        JS_ValueToUint16(cx, argv[2], &attrs);
    }

    jsval property;
    JS_GetProperty(cx, value, "get", &property);
    JS_ValueToObject(cx, property, &getter);
    JS_GetProperty(cx, value, "set", &property);
    JS_ValueToObject(cx, property, &setter);

    if (getter && JS_ObjectIsFunction(cx, getter)) {
        JS_DefineProperty(cx, object, name, JSVAL_VOID, (JSPropertyOp) getter, NULL, attrs|JSPROP_GETTER);
    }
    if (setter && JS_ObjectIsFunction(cx, setter)) {
        JS_DefineProperty(cx, object, name, JSVAL_VOID, NULL, (JSPropertyOp) setter, attrs|JSPROP_SETTER);
    }

    JS_LeaveLocalRootScope(cx);
    JS_EndRequest(cx);
    return JS_TRUE;
}

