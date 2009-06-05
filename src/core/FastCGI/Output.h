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

#ifndef _LULZJS_FCGI_OUTPUT_H
#define _LULZJS_FCGI_OUTPUT_H

#include "lulzjs.h"
#include "fcgiapp.h"
#include "common.h"

extern "C" JSBool Output_initialize (JSContext* cx);

static JSClass Output_class = {
    "Output", JSCLASS_HAS_PRIVATE,
    JS_PropertyStub, JS_PropertyStub, JS_PropertyStub, JS_PropertyStub,
    JS_EnumerateStub, JS_ResolveStub, JS_ConvertStub, JS_FinalizeStub
};

JSBool Output_content_get (JSContext *cx, JSObject *obj, jsval idval, jsval *vp);
JSBool Output_content_set (JSContext *cx, JSObject *obj, jsval idval, jsval *vp);

static JSPropertySpec Output_attributes[] = {
    {"content",  0, 0, Output_content_get, Output_content_set},

    {NULL}
};

JSBool Output_write (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);
JSBool Output_writeLine (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);
JSBool Output_flush (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);

static JSFunctionSpec Output_methods[] = {
    {"write",     Output_write, 0, 0, 0},
    {"writeLine", Output_writeLine, 0, 0, 0},
    {"flush",     Output_flush, 0, 0, 0},

    {NULL}
};

#endif
