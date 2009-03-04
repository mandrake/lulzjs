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

#ifndef _CORE_H
#define _CORE_H

#include "lulzjs.h"
#include "jsdbgapi.h"

#include <iostream>
#include <string>
#include <list>

// Not cross platform
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <libgen.h>
#include <dlfcn.h>

static JSClass Core_class = {
    "Core", JSCLASS_GLOBAL_FLAGS|JSCLASS_HAS_PRIVATE,
    JS_PropertyStub, JS_PropertyStub, JS_PropertyStub, JS_PropertyStub,
    JS_EnumerateStub, JS_ResolveStub, JS_ConvertStub, JS_FinalizeStub,
    JSCLASS_NO_OPTIONAL_MEMBERS
};

JSObject* Core_initialize (JSContext* cx, const char* script);

JSBool Core_include (JSContext* cx, JSObject* obj, uintN argc, jsval* argv, jsval* rval); 
JSBool Core_require (JSContext* cx, JSObject* obj, uintN argc, jsval* argv, jsval* rval);
JSBool Core_compile (JSContext* cx, JSObject* obj, uintN argc, jsval* argv, jsval* rval);
JSBool Core_GC (JSContext* cx, JSObject* obj, uintN argc, jsval* argv, jsval* rval);

JSBool Core_die (JSContext* cx, JSObject* obj, uintN argc, jsval* argv, jsval* rval);
JSBool Core_exit (JSContext* cx, JSObject* obj, uintN argc, jsval* argv, jsval* rval);

JSBool Core_exec (JSContext* cx, JSObject* obj, uintN argc, jsval* argv, jsval* rval);

JSBool Core_sleep (JSContext* cx, JSObject* obj, uintN argc, jsval* argv, jsval* rval);

JSBool Core_print (JSContext* cx, JSObject* obj, uintN argc, jsval* argv, jsval* rval);

static JSFunctionSpec Core_methods[] = {
    {"include", Core_include, 0, 0, 0},
    {"require", Core_require, 0, 0, 0},
    {"GC",      Core_GC,      0, 0, 0},

    {"die",  Core_die,  0, 0, 0},
    {"exit", Core_exit, 0, 0, 0},

    {"exec",  Core_exec, 0, 0, 0},
    {"sleep", Core_sleep, 0, 0, 0},

    {"print", Core_print, 0, 0, 0},
    {NULL}
};

#endif
