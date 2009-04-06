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
        JS_DefineProperties(cx, object, FileSystem_attributes);

        JS_EndRequest(cx);
        return JS_TRUE;
    }

    return JS_FALSE;
}

JSBool
FileSystem_umask_get (JSContext *cx, JSObject *obj, jsval idval, jsval *vp)
{
    mode_t mask = umask(0);
    umask(mask);

    char string[5];
    snprintf(string, 5, "%04o", mask);

    JS_BeginRequest(cx);
    *vp = STRING_TO_JSVAL(JS_NewString(cx, JS_strdup(cx, string), strlen(string)));
    JS_EndRequest(cx);
    return JS_TRUE;
}

JSBool
FileSystem_umask_set (JSContext *cx, JSObject *obj, jsval idval, jsval *vp)
{
    JS_BeginRequest(cx);
    int mask; JS_ValueToInt32(cx, *vp, &mask);
    JS_EndRequest(cx);

    umask(mask);

    return JS_TRUE;
}

