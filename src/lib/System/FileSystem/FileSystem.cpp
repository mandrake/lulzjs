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

#include "FileSystem.h"

JSBool exec (JSContext* cx) { return FileSystem_initialize(cx); }

JSBool
FileSystem_initialize (JSContext* cx)
{
    JS_BeginRequest(cx);

    jsval jsParent   = JS_EVAL(cx, "System");
    JSObject* parent = JSVAL_TO_OBJECT(jsParent);

    JSObject* object = JS_DefineObject(
        cx, parent,
        FileSystem_class.name, &FileSystem_class, NULL, 
        JSPROP_PERMANENT|JSPROP_READONLY|JSPROP_ENUMERATE);

    if (object) {
        JS_DefineFunctions(cx, object, FileSystem_methods);

        jsval property;

        property = INT_TO_JSVAL(EOF);
        JS_SetProperty(cx, parent, "EOF", &property);

        JS_EndRequest(cx);
        return JS_TRUE;
    }

    return JS_FALSE;
}

