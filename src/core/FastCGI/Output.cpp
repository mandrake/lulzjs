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

#include "Output.h"

JSBool
Output_initialize (JSContext* cx)
{
    JSBool result = JS_FALSE;

    JS_BeginRequest(cx);
    JS_EnterLocalRootScope(cx);

    jsval property;

    JSObject* output = JS_DefineObject(cx, ((LCGIData*) JS_GetContextPrivate(cx))->global, 
        "Output", &Output_class, NULL, 0
    );

    if (output) {
        property = INT_TO_JSVAL(1024);
        JS_SetProperty(cx, output, "limit", &property);
        property = JSVAL_FALSE;
        JS_SetProperty(cx, output, "buffered", &property);
        property = JS_GetEmptyStringValue(cx);
        JS_SetProperty(cx, output, "content", &property);

        result = JS_TRUE;
    }
    
    JS_LeaveLocalRootScope(cx);
    JS_EndRequest(cx);

    return result;
}

JSBool
Output_content_get (JSContext *cx, JSObject *obj, jsval idval, jsval *vp)
{
    return JS_TRUE;
}

JSBool
Output_content_set (JSContext *cx, JSObject *obj, jsval idval, jsval *vp)
{
    JS_BeginRequest(cx);
    JS_EnterLocalRootScope(cx);

    jsval     property;
    int       limit;
    JSBool    buffered;
    JSString* string;

    JS_GetProperty(cx, obj, "limit", &property);
    JS_ValueToInt32(cx, property, &limit);
    JS_GetProperty(cx, obj, "buffered", &property);
    JS_ValueToBoolean(cx, property, &buffered);
    string = JS_ValueToString(cx, *vp);

    if (!buffered && JS_GetStringLength(string) > limit) {
        if (!JSVAL_TO_BOOLEAN(JS_EVAL(cx, "Headers.sent"))) {
            JS_EVAL(cx, "Headers.send()");
        }

        FCGX_Stream* out       = ((LCGIData*) JS_GetContextPrivate(cx))->cgi->out;
        char*        cString   = JS_GetStringBytes(string);
        char         size[300] = {NULL};
        sprintf(size, "%x", strlen(cString));

        FCGX_FPrintF(out, "%s\r\n%s\r\n", size, cString);

        property = JS_GetEmptyStringValue(cx);
        JS_SetProperty(cx, obj, "content", &property);
    }

    JS_LeaveLocalRootScope(cx);
    JS_EndRequest(cx);

    return JS_TRUE;
}

JSBool
Output_write (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    JS_BeginRequest(cx);
    JS_EnterLocalRootScope(cx);

    JSString* string;

    if (argc < 1 || !JS_ConvertArguments(cx, argc, argv, "S", &string)) {
        JS_ReportError(cx, "Not enough parameters.");
        JS_LeaveLocalRootScope(cx);
        JS_EndRequest(cx);
        return JS_FALSE;
    }

    jsval property;

    JS_GetProperty(cx, obj, "content", &property);
    property = STRING_TO_JSVAL(JS_ConcatStrings(cx, JS_ValueToString(cx, property), string));
    JS_SetProperty(cx, obj, "content", &property);

    JS_LeaveLocalRootScope(cx);
    JS_EndRequest(cx);

    return JS_TRUE;
}

JSBool
Output_writeLine (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    JS_BeginRequest(cx);
    JS_EnterLocalRootScope(cx);

    JSString* string;
    JSString* endLine = JS_NewStringCopyZ(cx, "\n");

    if (argc < 1 || !JS_ConvertArguments(cx, argc, argv, "S", &string)) {
        JS_ReportError(cx, "Not enough parameters.");
        JS_LeaveLocalRootScope(cx);
        JS_EndRequest(cx);
        return JS_FALSE;
    }

    jsval property;

    string = JS_ConcatStrings(cx, string, endLine);
    JS_GetProperty(cx, obj, "content", &property);
    property = STRING_TO_JSVAL(JS_ConcatStrings(cx, JS_ValueToString(cx, property), string));
    JS_SetProperty(cx, obj, "content", &property);

    JS_LeaveLocalRootScope(cx);
    JS_EndRequest(cx);

    return JS_TRUE;
}


JSBool
Output_flush (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    JS_BeginRequest(cx);
    JS_EnterLocalRootScope(cx);

    jsval property;
    JS_GetProperty(cx, obj, "content", &property);

    FCGX_PutS(JS_GetStringBytes(JS_ValueToString(cx, property)), ((LCGIData*) JS_GetContextPrivate(cx))->cgi->out);

    property = JS_GetEmptyStringValue(cx);
    JS_SetProperty(cx, obj, "content", &property);

    JS_LeaveLocalRootScope(cx);
    JS_EndRequest(cx);

    return JS_TRUE;
}

