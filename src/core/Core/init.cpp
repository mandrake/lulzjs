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

#include "lulzjs.h"

extern "C"
JSBool
exec (JSContext* cx)
{
    JSObject* PATH; JS_ValueToObject(cx, JS_EVAL(cx, "__PATH__"), &PATH);
    char* value  = NULL;

    JS_BeginRequest(cx);
    JS_EnterLocalRootScope(cx);

    if ((value = getenv("JSPATH")) != NULL) {
        char* check = NULL;

        while ((check = strchr(value, ':')) != NULL) {
            jsval newArgv[] = {STRING_TO_JSVAL(JS_NewString(cx, 
                JS_strndup(cx, value, abs(value-check)), abs(value-check)
            ))}; jsval ret;

            JS_CallFunctionName(cx, PATH, "push", 1, newArgv, &ret);
            value = check+1;
        }
        
        jsval newArgv[] = {STRING_TO_JSVAL(JS_NewString(cx, 
            JS_strdup(cx, value), strlen(value)
        ))}; jsval ret;

        JS_CallFunctionName(cx, PATH, "push", 1, newArgv, &ret);
    }

    if ((value = getenv("JSINCLUDE")) != NULL) {
        char* check = NULL;

        while ((check = strchr(value, ':')) != NULL) {
            jsval newArgv[] = {STRING_TO_JSVAL(JS_NewString(cx, 
                JS_strndup(cx, value, abs(value-check)), abs(value-check)
            ))}; jsval ret;

            JS_CallFunctionName(cx, JS_GetGlobalObject(cx), "include", 1, newArgv, &ret);
            value = check+1;
        }
        
        jsval newArgv[] = {STRING_TO_JSVAL(JS_NewString(cx, 
            JS_strdup(cx, value), strlen(value)
        ))}; jsval ret;

        JS_CallFunctionName(cx, JS_GetGlobalObject(cx), "include", 1, newArgv, &ret);
    }

    JS_LeaveLocalRootScope(cx);
    JS_EndRequest(cx);

    return JS_TRUE;
}

