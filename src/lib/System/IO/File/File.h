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

#ifndef _SYSTEM_IO_FILE_H
#define _SYSTEM_IO_FILE_H

#include "lulzjs.h"

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

JSBool File_close (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval);

JSBool File_write (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval);
JSBool File_read (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval);

JSBool File_writeBytes (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval);
JSBool File_readBytes (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval);

JSBool File_isEnd (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval);

JSBool File_static_exists (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval);

static JSFunctionSpec File_methods[] = {
    {"close", File_close, 0, 0, 0},

    {"write", File_write, 0, 0, 0},
    {"read",  File_read,  0, 0, 0},

    {"writeBytes", File_writeBytes, 0, 0, 0},
    {"readBytes",  File_readBytes,  0, 0, 0},

    {"isEnd", File_isEnd, 0, 0, 0},
    {NULL}
};

static JSFunctionSpec File_static_methods[] = {
    {"exists", File_static_exists, 0, 0, 0},
    {NULL}
};

#endif
