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

#ifndef _SYSTEM_NETWORK_SOCKETS_ICMP_H
#define _SYSTEM_NETWORK_SOCKETS_ICMP_H

#include "Sockets.h"

extern "C" JSBool exec (JSContext* cx);
JSBool ICMP_initialize (JSContext* cx);

JSBool ICMP_constructor (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval);
void  ICMP_finalize (JSContext* cx, JSObject* object); 

static JSClass ICMP_class = {
    "ICMP", JSCLASS_HAS_PRIVATE,
    JS_PropertyStub, JS_PropertyStub, JS_PropertyStub, JS_PropertyStub,
    JS_EnumerateStub, JS_ResolveStub, JS_ConvertStub, ICMP_finalize
};

#include "ICMP_private.h"

static JSPropertySpec ICMP_attributes[] = {
    {NULL}
};

static JSFunctionSpec ICMP_methods[] = {
    JS_FS_END
};

static JSFunctionSpec ICMP_static_methods[] = {
    JS_FS_END
};

#endif
