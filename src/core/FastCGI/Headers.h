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

#ifndef _LULZJS_FCGI_HEADERS_H
#define _LULZJS_FCGI_HEADERS_H

#include "lulzjs.h"
#include "fcgiapp.h"
#include "common.h"

typedef struct {
    JSBool sent;
} HeadersInformation;

extern "C" JSBool Headers_initialize (JSContext* cx);

JSBool Headers_set (JSContext *cx, JSObject *obj, jsval id, jsval *vp);

static JSClass Headers_class = {
    "Headers", JSCLASS_HAS_PRIVATE,
    JS_PropertyStub, JS_PropertyStub, JS_PropertyStub, Headers_set,
    JS_EnumerateStub, JS_ResolveStub, JS_ConvertStub, JS_FinalizeStub
};

JSBool Headers_send (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);

static JSFunctionSpec Headers_methods[] = {
    {"send", Headers_send, 0, 0, 0},
    {NULL}
};

#endif
