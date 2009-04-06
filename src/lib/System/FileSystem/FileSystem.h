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

#ifndef _SYSTEM_FILESYSTEM_H
#define _SYSTEM_FILESYSTEM_H

#include "lulzjs.h"

extern "C" JSBool exec (JSContext* cx);
JSBool FileSystem_initialize (JSContext* cx);

static JSClass FileSystem_class = {
    "FileSystem", 0,
    JS_PropertyStub, JS_PropertyStub, JS_PropertyStub, JS_PropertyStub,
    JS_EnumerateStub, JS_ResolveStub, JS_ConvertStub, JS_FinalizeStub
};

JSBool FileSystem_umask_get (JSContext *cx, JSObject *obj, jsval idval, jsval *vp);
JSBool FileSystem_umask_set (JSContext *cx, JSObject *obj, jsval idval, jsval *vp);

static JSPropertySpec FileSystem_attributes[] = {
    {"umask", 0, 0, FileSystem_umask_get, FileSystem_umask_set},
    {NULL}
};

static JSFunctionSpec FileSystem_methods[] = {
    {NULL}
};

#endif
