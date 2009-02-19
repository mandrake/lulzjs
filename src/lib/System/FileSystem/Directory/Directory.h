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

#ifndef _SYSTEM_FILESYSTEM_DIRECTORY_H
#define _SYSTEM_FILESYSTEM_DIRECTORY_H

#include "lulzjs.h"
#include <dirent.h>

extern "C" JSBool exec (JSContext* cx);
JSBool Directory_initialize (JSContext* cx);

JSBool Directory_constructor (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval);
void  Directory_finalize (JSContext* cx, JSObject* object); 

static JSClass Directory_class = {
    "Directory", JSCLASS_HAS_PRIVATE,
    JS_PropertyStub, JS_PropertyStub, JS_PropertyStub, JS_PropertyStub,
    JS_EnumerateStub, JS_ResolveStub, JS_ConvertStub, Directory_finalize
};

JSBool Directory_position_get (JSContext *cx, JSObject *obj, jsval idval, jsval *vp);
JSBool Directory_position_set (JSContext *cx, JSObject *obj, jsval idval, jsval *vp);

JSBool Directory_length_get (JSContext *cx, JSObject *obj, jsval idval, jsval *vp);

static JSPropertySpec Directory_attributes[] = {
    {"position", 0, 0, Directory_position_get, Directory_position_set},
    {"length",   0, 0, Directory_length_get,   NULL},
    {NULL}
};

#include "private.h"

JSBool Directory_open (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval);
JSBool Directory_close (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval);

JSBool Directory_next (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval);
JSBool Directory_previous (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval);

static JSFunctionSpec Directory_methods[] = {
    {"open",  Directory_open,  0, 0, 0},
    {"close", Directory_close, 0, 0, 0},

    {"next",     Directory_next, 0, 0, 0},
    {"previous", Directory_previous, 0, 0, 0},

    {NULL}
};

static JSFunctionSpec Directory_static_methods[] = {
    {NULL}
};

#endif
