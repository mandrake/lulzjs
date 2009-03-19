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

#ifndef _SYSTEM_NET_SOCKET_H
#define _SYSTEM_NET_SOCKET_H

#include "lulzjs.h"
#include "prnetdb.h"
#include "prinrval.h"

extern "C" JSBool exec (JSContext* cx);
JSBool Sockets_initialize (JSContext* cx);

static JSClass Sockets_class = {
    "Sockets", 0,
    JS_PropertyStub, JS_PropertyStub, JS_PropertyStub, JS_PropertyStub,
    JS_EnumerateStub, JS_ResolveStub, JS_ConvertStub, JS_FinalizeStub
};

#include "private.h"

JSBool Sockets_connect (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval);

JSBool Sockets_listen (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval);
JSBool Sockets_accept (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval);

JSBool Sockets_write (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval);
JSBool Sockets_writeTo (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval);
JSBool Sockets_read (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval);
JSBool Sockets_readFrom (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval);

JSBool Sockets_static_getHostByName (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval);

JSBool Sockets_static_isIPv4 (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval);

static JSFunctionSpec Sockets_methods[] = {
    {"connect", Sockets_connect, 0, 0, 0},

    {"listen", Sockets_listen, 0, 0, 0},
    {"accept", Sockets_accept, 0, 0, 0},

    {"write",    Sockets_write,    0, 0, 0},
    {"writeTo",  Sockets_writeTo,  0, 0, 0},
    {"read",     Sockets_read,     0, 0, 0},
    {"readFrom", Sockets_readFrom, 0, 0, 0},
    {NULL}
};

static JSFunctionSpec Sockets_static_methods[] = {
    {"getHostByName", Sockets_static_getHostByName, 0, 0, 0},
    {"isIPv4",        Sockets_static_isIPv4,        0, 0, 0},

    {NULL}
};

#endif
