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

#include "ICMP.h"

JSBool exec (JSContext* cx) { return ICMP_initialize(cx); }

JSBool
ICMP_initialize (JSContext* cx)
{
    JS_BeginRequest(cx);
    JS_EnterLocalRootScope(cx);

    JSObject* parent = JSVAL_TO_OBJECT(JS_EVAL(cx, "System.Network.Sockets"));

    JSObject* object = JS_InitClass(
        cx, parent, parent, &ICMP_class,
        ICMP_constructor, 1, ICMP_attributes, ICMP_methods, NULL, ICMP_static_methods
    );

    if (object) {
        JS_LeaveLocalRootScope(cx);
        JS_EndRequest(cx);
        return JS_TRUE;
    }

    return JS_FALSE;
}

JSBool
ICMP_constructor (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval)
{
    ICMPInformation* data = new ICMPInformation;
    JS_SetPrivate(cx, object, data);
    data->socket = NULL;

    return JS_TRUE;
}

void
ICMP_finalize (JSContext* cx, JSObject* object)
{
    JS_BeginRequest(cx);

    ICMPInformation* data = (ICMPInformation*) JS_GetPrivate(cx, object);

    if (data) {
        delete data;
    }

    JS_EndRequest(cx);
}

