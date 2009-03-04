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

#include "Environment.h"

JSBool exec (JSContext* cx) { return Environment_initialize(cx); }

JSBool
Environment_initialize (JSContext* cx)
{
    JS_BeginRequest(cx);
    JS_EnterLocalRootScope(cx);

    JSObject* object = JS_DefineObject(
        cx, JSVAL_TO_OBJECT(JS_EVAL(cx, "System")),
        Environment_class.name, &Environment_class, NULL, 
        JSPROP_PERMANENT|JSPROP_READONLY|JSPROP_ENUMERATE
    );

    if (object) {
        JS_DefineFunctions(cx, object, Environment_methods);

        JS_LeaveLocalRootScope(cx);
        JS_EndRequest(cx);
        return JS_TRUE;
    }

    return JS_FALSE;
}

JSBool
Environment_get (JSContext *cx, JSObject *obj, jsval id, jsval *vp)
{
    JS_BeginRequest(cx);
    JS_EnterLocalRootScope(cx);

    jsval jsName; JS_IdToValue(cx, id, &jsName);
    std::string name = JS_GetStringBytes(JS_ValueToString(cx, jsName));

    char* value = getenv(name.c_str());

    *vp = (value)
        ? STRING_TO_JSVAL(JS_NewString(cx, JS_strdup(cx, value), strlen(value)))
        : JSVAL_NULL;

    JS_LeaveLocalRootScope(cx);
    JS_EndRequest(cx);
    return JS_TRUE;
}

JSBool
Environment_set (JSContext *cx, JSObject *obj, jsval id, jsval *vp)
{
    JS_BeginRequest(cx);
    JS_EnterLocalRootScope(cx);

    jsval jsName; JS_IdToValue(cx, id, &jsName);
    std::string name = JS_GetStringBytes(JS_ValueToString(cx, jsName));

    setenv(name.c_str(), JS_GetStringBytes(JS_ValueToString(cx, *vp)), 1);

    JS_LeaveLocalRootScope(cx);
    JS_EndRequest(cx);
    return JS_TRUE;
}

JSBool
Environment_del (JSContext *cx, JSObject *obj, jsval id, jsval *vp)
{
    JS_BeginRequest(cx);
    JS_EnterLocalRootScope(cx);

    jsval jsName; JS_IdToValue(cx, id, &jsName);
    std::string name = JS_GetStringBytes(JS_ValueToString(cx, jsName));

    unsetenv(name.c_str());

    JS_LeaveLocalRootScope(cx);
    JS_EndRequest(cx);
    return JS_TRUE;
}

