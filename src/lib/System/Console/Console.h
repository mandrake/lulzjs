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

#ifndef _SYSTEM_CONSOLE_H
#define _SYSTEM_CONSOLE_H

#include "lulzjs.h"

extern JSBool exec (JSContext* cx);
extern JSBool Console_initialize (JSContext* cx);

static JSClass Console_class = {
    "Console", 0,
    JS_PropertyStub, JS_PropertyStub, JS_PropertyStub, JS_PropertyStub,
    JS_EnumerateStub, JS_ResolveStub, JS_ConvertStub, JS_FinalizeStub
};

extern JSBool Console_write (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval);
extern JSBool Console_error (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval);
extern JSBool Console_readLine (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval);

static JSFunctionSpec Console_methods[] = {
    {"write",    Console_write,    0, 0, 0},
    {"error",    Console_error,    0, 0, 0},
    {"readLine", Console_readLine, 0, 0, 0},
    {NULL}
};

#endif
