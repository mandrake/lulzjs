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

#include "Headers.h"

JSBool
Headers_initialize (JSContext* cx)
{
    JSBool result = JS_FALSE;

    JS_BeginRequest(cx);
    JS_EnterLocalRootScope(cx);

    jsval property;
    char* stuff;

    JSObject* headers = JS_DefineObject(
        cx, ((LCGIData*) JS_GetContextPrivate(cx))->global, 
        "Headers", &Headers_class, NULL, 0
    );

    if (headers) {
        property = JSVAL_FALSE;
        JS_SetProperty(cx, headers, "sent", &property);
        property = STRING_TO_JSVAL(JS_NewStringCopyZ(cx, "200 OK"));
        JS_SetProperty(cx, headers, "Status", &property);
        property = STRING_TO_JSVAL(JS_NewStringCopyZ(cx, "text/html"));
        JS_SetProperty(cx, headers, "Content-Type", &property);
        property = STRING_TO_JSVAL(JS_NewStringCopyZ(cx, "chunked"));
        JS_SetProperty(cx, headers, "Transfer-Encoding", &property);
        property = STRING_TO_JSVAL(JS_NewStringCopyZ(cx, "lulzJS/" __LJS_VERSION__));
        JS_SetProperty(cx, headers, "X-Powered-By", &property);

        if ((stuff = getenv("SERVER_SOFTWARE"))) {
            property = STRING_TO_JSVAL(JS_NewStringCopyZ(cx, stuff));
            JS_SetProperty(cx, headers, "Server", &property);
        }

        property = JSVAL_FALSE;
        JS_SetProperty(cx, headers, "sent", &property);

        result = JS_TRUE;
    }
    
    JS_LeaveLocalRootScope(cx);
    JS_EndRequest(cx);

    return result;
}

JSBool
Headers_set (JSContext *cx, JSObject *obj, jsval id, jsval *vp)
{
    JS_BeginRequest(cx);
    
    jsval property;
    JS_GetProperty(cx, obj, "sent", &property);

    if (property == JSVAL_TRUE) {
        JS_ReportError(cx, "Headers have already been sent.");
        return JS_FALSE;
    }

    JS_EndRequest(cx);

    return JS_TRUE;
}

JSBool
Headers_send (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    JS_BeginRequest(cx);
    JS_EnterLocalRootScope(cx);

    int   i;
    jsval name;
    jsval value;
    jsval property;

    JS_GetProperty(cx, obj, "sent", &property);

    if (property == JSVAL_TRUE) {
        JS_ReportError(cx, "Headers have already been sent.");
        return JS_FALSE;
    }

    LCGIData* data = (LCGIData*) JS_GetContextPrivate(cx);

    FCGX_Stream* out    = data->cgi->out;
    JSObject*    global = data->global;

    jsval headers;
    JS_GetProperty(cx, global, "Headers", &headers);

    JSIdArray* ids = JS_Enumerate(cx, JSVAL_TO_OBJECT(headers));

    for (i = 0; i < ids->length; i++) {
        JS_IdToValue(cx, ids->vector[i], &name);
        JS_GetPropertyById(cx, global, ids->vector[i], &value);

        FCGX_FPrintF(out, "%s: %s\r\n",
            JS_GetStringBytes(JS_ValueToString(cx, name)),
            JS_GetStringBytes(JS_ValueToString(cx, value))
        );
    }
    FCGX_PutS("\r\n", out);
    
    JS_DestroyIdArray(cx, ids);

    JS_LeaveLocalRootScope(cx);
    JS_EndRequest(cx);
}

