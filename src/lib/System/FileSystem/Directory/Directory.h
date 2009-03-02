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
#include <vector>
#include <dirent.h>

const int SEARCH_EVERYTHING  = 0x01;
const int SEARCH_FILES       = 0x02;
const int SEARCH_DIRECTORIES = 0x04;

extern "C" JSBool exec (JSContext* cx);
JSBool Directory_initialize (JSContext* cx);

JSBool Directory_constructor (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval);
void  Directory_finalize (JSContext* cx, JSObject* object); 

static JSClass Directory_class = {
    "Directory", JSCLASS_HAS_PRIVATE,
    JS_PropertyStub, JS_PropertyStub, JS_PropertyStub, JS_PropertyStub,
    JS_EnumerateStub, JS_ResolveStub, JS_ConvertStub, Directory_finalize
};

JSBool Directory_name_get (JSContext *cx, JSObject *obj, jsval idval, jsval *vp);
JSBool Directory_path_get (JSContext *cx, JSObject *obj, jsval idval, jsval *vp);
JSBool Directory_length_get (JSContext *cx, JSObject *obj, jsval idval, jsval *vp);

JSBool Directory_position_get (JSContext *cx, JSObject *obj, jsval idval, jsval *vp);
JSBool Directory_position_set (JSContext *cx, JSObject *obj, jsval idval, jsval *vp);

JSBool Directory_size_get (JSContext *cx, JSObject *obj, jsval idval, jsval *vp);
JSBool Directory_permission_get (JSContext *cx, JSObject *obj, jsval idval, jsval *vp);
JSBool Directory_last_get (JSContext *cx, JSObject *obj, jsval idval, jsval *vp);

static JSPropertySpec Directory_attributes[] = {
    {"path",   0, 0, Directory_path_get,   NULL},
    {"length", 0, 0, Directory_length_get, NULL},

    {"position", 0, 0, Directory_position_get, Directory_position_set},

    {"size",       0, 0, Directory_size_get,       NULL},
    {"permission", 0, 0, Directory_permission_get, NULL},
    {"last",       0, 0, Directory_last_get,       NULL},
    {NULL}
};

#include "private.h"

JSBool Directory_open (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval);
JSBool Directory_close (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval);

JSBool Directory_fileAt (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval);

JSBool Directory_static_create (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval);

static JSFunctionSpec Directory_methods[] = {
    {"open",  Directory_open,  0, 0, 0},
    {"close", Directory_close, 0, 0, 0},

    {"fileAt", Directory_fileAt, 0, 0, 0},
    {NULL}
};

static JSFunctionSpec Directory_static_methods[] = {
    {"create", Directory_static_create, 0, 0, 0},
    {NULL}
};

#endif
