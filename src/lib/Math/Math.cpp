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

#include "Math.h"

JSBool exec (JSContext* cx) { return Math_initialize(cx); }

JSBool
Math_initialize (JSContext* cx)
{
    JS_BeginRequest(cx);

    JSObject* object = JSVAL_TO_OBJECT(JS_EVAL(cx, "Math"));

    if (object) {
        JS_DefineFunctions(cx, object, Math_methods);

        JS_EndRequest(cx);
        return JS_TRUE;
    }

    return JS_FALSE;
}

