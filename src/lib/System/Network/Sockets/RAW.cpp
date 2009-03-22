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

#include "RAW.h"

JSBool exec (JSContext* cx) { return RAW_initialize(cx); }

JSBool
RAW_initialize (JSContext* cx)
{
    JS_BeginRequest(cx);
    JS_EnterLocalRootScope(cx);

    JSObject* parent = JSVAL_TO_OBJECT(JS_EVAL(cx, "System.Network.Sockets"));

    JSObject* object = JS_InitClass(
        cx, parent, parent, &RAW_class,
        RAW_constructor, 1, RAW_attributes, RAW_methods, NULL, RAW_static_methods
    );

    if (object) {
        JS_LeaveLocalRootScope(cx);
        JS_EndRequest(cx);
        return JS_TRUE;
    }

    return JS_FALSE;
}

JSBool
RAW_constructor (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval)
{
    RAWInformation* data = new RAWInformation;
    JS_SetPrivate(cx, object, data);
    data->socket = NULL;

    return JS_TRUE;
}

void
RAW_finalize (JSContext* cx, JSObject* object)
{
    JS_BeginRequest(cx);

    RAWInformation* data = (RAWInformation*) JS_GetPrivate(cx, object);

    if (data) {
        delete data;
    }

    JS_EndRequest(cx);
}

