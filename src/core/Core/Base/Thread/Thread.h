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

#ifndef _SYSTEM_THREAD_H
#define _SYSTEM_THREAD_H

#include "jsapi.h"

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern "C" JSBool exec (JSContext* cx);
void reportError (JSContext *cx, const char *message, JSErrorReport *report);

JSBool Thread_initialize (JSContext* cx);

JSBool Thread_constructor (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval);
void   Thread_finalize (JSContext* cx, JSObject* object); 

static JSClass Thread_class = {
    "Thread", JSCLASS_HAS_PRIVATE,
    JS_PropertyStub, JS_PropertyStub, JS_PropertyStub, JS_PropertyStub,
    JS_EnumerateStub, JS_ResolveStub, JS_ConvertStub, Thread_finalize
};

#include "private.h"

JSBool Thread_start (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval);

JSBool Thread_join (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval);

JSBool Thread_stop (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval);

JSBool Thread_static_cancel (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval);
JSBool Thread_static_cancelPoint (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval);

static JSFunctionSpec Thread_methods[] = {
    {"start", Thread_start, 0, 0, 0},
    {"join",  Thread_join,  0, 0, 0},
    {"stop",  Thread_stop,  0, 0, 0},
    {NULL}
};

static JSFunctionSpec Thread_static_methods[] = {
    {"cancel",      Thread_static_cancel,      0, 0, 0},
    {"cancelPoint", Thread_static_cancelPoint, 0, 0, 0},
    {NULL}
};

#endif
