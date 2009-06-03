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

void
Headers_initialize (JSContext* cx)
{
    JS_BeginRequest(cx);
    JS_EnterLocalRootScope(cx);

    jsval property;
    char* stuff;

    JSObject* headers = JS_NewObject(NULL, NULL, NULL, NULL);
    property          = OBJECT_TO_JSVAL(headers);
    JS_SetProperty(engine.context, engine.core, "Headers", &property);
        property = JSVAL_FALSE;
        JS_SetProperty(engine.context, headers, "sent", &property);
        property = STRING_TO_JSVAL(JS_NewStringCopyZ(engine.context, "200 OK"));
        JS_SetProperty(engine.context, headers, "Status", &property);
        property = STRING_TO_JSVAL(JS_NewStringCopyZ(engine.context, "text/html"));
        JS_SetProperty(engine.context, headers, "Content-Type", &property);
        property = STRING_TO_JSVAL(JS_NewStringCopyZ(engine.context, "chunked"));
        JS_SetProperty(engine.context, headers, "Transfer-Encoding", &property);
        property = STRING_TO_JSVAL(JS_NewStringCopyZ(engine.context, "lulzJS/" __LJS_VERSION__));
        JS_SetProperty(engine.context, headers, "X-Powered-By", &property);
        stuff = getenv("SERVER_SOFTWARE");
        if (stuff) {
            property = STRING_TO_JSVAL(JS_NewStringCopyZ(engine.context, stuff));
            JS_SetProperty(engine.context, headers, "Server", &property);
        }

    
}

JSBool
Headers_send (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    JS_BeginRequest(cx);
    JS_EnterLocalRootScope(cx);

    int   i;
    jsval name;
    jsval value;

    HeadersInformation* data = (HeadersInformation*) JS_GetPrivate(cx, obj);
    data->sent = JS_TRUE;

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
