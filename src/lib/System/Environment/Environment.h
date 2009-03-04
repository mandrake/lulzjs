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

#ifndef _SYSTEM_H
#define _SYSTEM_H

#include "lulzjs.h"

extern "C" JSBool exec (JSContext* cx);
JSBool Environment_initialize (JSContext* cx);

JSBool Environment_get (JSContext *cx, JSObject *obj, jsval id, jsval *vp);
JSBool Environment_set (JSContext *cx, JSObject *obj, jsval id, jsval *vp);
JSBool Environment_del (JSContext *cx, JSObject *obj, jsval id, jsval *vp);

static JSClass Environment_class = {
    "Environment", 0,
    JS_PropertyStub, Environment_del, Environment_get, Environment_set,
    JS_EnumerateStub, JS_ResolveStub, JS_ConvertStub, JS_FinalizeStub
};

static JSFunctionSpec Environment_methods[] = {
    {NULL}
};

#endif
