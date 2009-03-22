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

#include "Hashing.h"

JSBool exec (JSContext* cx) { return Hashing_initialize(cx); }

JSBool
Hashing_initialize (JSContext* cx)
{
    JSObject* parent = JSVAL_TO_OBJECT(JS_EVAL(cx, "Math.Cryptography"));

    JSObject* object = JS_DefineObject(
        cx, parent,
        Hashing_class.name, &Hashing_class, NULL, 
        JSPROP_PERMANENT|JSPROP_READONLY|JSPROP_ENUMERATE);

    if (object) {
        JS_DefineFunctions(cx, object, Hashing_methods);

        return JS_TRUE;
    }

    return JS_FALSE;
}

