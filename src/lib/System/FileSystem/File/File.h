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

#ifndef _SYSTEM_FILESYSTEM_FILE_H
#define _SYSTEM_FILESYSTEM_FILE_H

#undef  _GNU_SOURCE
#undef  _FILE_OFFSET_BITS
#define _GNU_SOURCE
#define _FILE_OFFSET_BITS 64

#include "lulzjs.h"

const unsigned MODE_READ   = 0x02;
const unsigned MODE_WRITE  = 0x04;
const unsigned MODE_APPEND = 0x08;

extern "C" JSBool exec (JSContext* cx);
JSBool File_initialize (JSContext* cx);

JSBool File_constructor (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval);
void  File_finalize (JSContext* cx, JSObject* object); 

static JSClass File_class = {
    "File", JSCLASS_HAS_PRIVATE,
    JS_PropertyStub, JS_PropertyStub, JS_PropertyStub, JS_PropertyStub,
    JS_EnumerateStub, JS_ResolveStub, JS_ConvertStub, File_finalize
};

#include "private.h"

JSBool File_path_get (JSContext *cx, JSObject *obj, jsval idval, jsval *vp);

JSBool File_position_get (JSContext *cx, JSObject *obj, jsval idval, jsval *vp);
JSBool File_position_set (JSContext *cx, JSObject *obj, jsval idval, jsval *vp);

JSBool File_length_get (JSContext *cx, JSObject *obj, jsval idval, jsval *vp);

static JSPropertySpec File_attributes[] = {
    {"path",     0, 0, File_path_get,     NULL},
    {"position", 0, 0, File_position_get, File_position_set},
    {"length",   0, 0, File_length_get,   NULL},
    {NULL}
};

JSBool File_open (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval);
JSBool File_close (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval);

JSBool File_write (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval);
JSBool File_read (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval);

JSBool File_writeBytes (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval);
JSBool File_readBytes (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval);

static JSFunctionSpec File_methods[] = {
    {"open",  File_open,  0, 0, 0},
    {"close", File_close, 0, 0, 0},

    {"write", File_write, 0, 0, 0},
    {"read",  File_read,  0, 0, 0},

    {"writeBytes", File_writeBytes, 0, 0, 0},
    {"readBytes",  File_readBytes,  0, 0, 0},
    {NULL}
};

static JSFunctionSpec File_static_methods[] = {
    {NULL}
};

#endif
