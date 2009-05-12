/****************************************************************************
* This file is part of lulzJS                                               *
* Copyleft meh. [http://meh.doesntexist.org | meh.ffff@gmail.com]           *
*                                                                           *
* lulzJS is free software: you can redistribute it and/or modify            *
* it under the terms of the GNU General Public License as published by      *
* the Free Software Foundation, either version 3 of the License, or         *
* (at your option) any later version.                                       *
*                                                                           *
* lulzJS is distributed in the hope that it will be usefull                 *
* but WITHOUT ANY WARRANTY; without even the implied warranty of            *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
* GNU General Public License for more details.                              *
*                                                                           *
* You should have received a copy of the GNU General Public License         *
* along with lulzJS.  If not, see <http://www.gnu.org/licenses/>.           *
****************************************************************************/

#include "Sockets.h"
#include "Sockets_common.cpp"

JSBool exec (JSContext* cx) { return Sockets_initialize(cx); }

JSBool
Sockets_initialize (JSContext* cx)
{
    JS_BeginRequest(cx);
    JS_EnterLocalRootScope(cx);

    JSObject* parent = JSVAL_TO_OBJECT(JS_EVAL(cx, "System.Network"));

    JSObject* object = JS_DefineObject(
        cx, parent,
        Sockets_class.name, &Sockets_class, NULL,
        JSPROP_PERMANENT|JSPROP_READONLY|JSPROP_ENUMERATE
    );

    if (object) {
        jsval property;

        JSObject* prototype = JS_NewObject(cx, NULL, NULL, NULL);
        property = OBJECT_TO_JSVAL(prototype);
        JS_SetProperty(cx, object, "prototype", &property);
            JS_DefineFunctions(cx, prototype, Sockets_methods);
            property = JSVAL_FALSE;
            JS_SetProperty(cx, prototype, "connected", &property);

        JSObject* static_prototype = JS_NewObject(cx, NULL, NULL, NULL);
        property = OBJECT_TO_JSVAL(static_prototype);
        JS_SetProperty(cx, object, "static_prototype", &property);
            JS_DefineFunctions(cx, static_prototype, Sockets_static_inherited_methods);

        JS_DefineFunctions(cx, object, Sockets_static_methods);

        JS_LeaveLocalRootScope(cx);
        JS_EndRequest(cx);
        return JS_TRUE;
    }

    return JS_FALSE;
}

JSBool
Sockets_writeTo (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval)
{
    JSString* stringObject;
    uint16    flags   = 0;
    jsdouble  timeout = -1;

    JS_BeginRequest(cx);

    if (argc < 1) {
        JS_ReportError(cx, "Not enough parameters.");
        JS_EndRequest(cx);
        return JS_FALSE;
    }

    JS_EnterLocalRootScope(cx);

    switch (argc) {
        case 3: JS_ValueToNumber(cx, argv[2], &timeout);
        case 2: JS_ValueToUint16(cx, argv[1], &flags);
        case 1: stringObject = JS_ValueToString(cx, argv[0]);
    }

    SocketsInformation* data = (SocketsInformation*) JS_GetPrivate(cx, object);

    jsval jsConnected; JS_GetProperty(cx, object, "connected", &jsConnected);
    JSBool connected; JS_ValueToBoolean(cx, jsConnected, &connected);

    if (!connected) {
        JS_ReportError(cx, "The socket isn't connected.");
        JS_LeaveLocalRootScope(cx);
        JS_EndRequest(cx);
        return JS_FALSE;
    }

    char*  string = JS_GetStringBytes(stringObject); 
    size_t offset = 0;
    int    sent   = 0;
    size_t length = JS_GetStringLength(stringObject);

    jsrefcount req = JS_SuspendRequest(cx);
    while (offset < length) {
        sent = PR_Send(data->socket, (string+offset), (length-offset)*sizeof(char), flags,
            (timeout == -1) ? PR_INTERVAL_NO_TIMEOUT : PR_MicrosecondsToInterval(timeout*1000000));

        if (sent < 0) {
            break;
        }
    }
    JS_ResumeRequest(cx, req);
    JS_LeaveLocalRootScope(cx);

    if (sent < 0) {
        PR_THROW_ERROR(cx);
        JS_EndRequest(cx);
        return JS_FALSE;
    }

    JS_EndRequest(cx);
    return JS_TRUE;
}

JSBool
Sockets_readFrom (JSContext *cx, JSObject *object, uintN argc, jsval *argv, jsval *rval)
{
    int32    size;
    uint16   flags   = 0;
    jsdouble timeout = -1;

    JS_BeginRequest(cx);

    if (argc < 1) {
        JS_ReportError(cx, "Not enough parameters.");
        JS_EndRequest(cx);
        return JS_FALSE;
    }

    JS_EnterLocalRootScope(cx);

    switch (argc) {
        case 3: JS_ValueToNumber(cx, argv[2], &timeout);
        case 2: JS_ValueToUint16(cx, argv[1], &flags);
        case 1: JS_ValueToInt32(cx, argv[0], &size);
    }

    SocketsInformation* data = (SocketsInformation*) JS_GetPrivate(cx, object);

    jsval jsConnected; JS_GetProperty(cx, object, "connected", &jsConnected);
    JSBool connected; JS_ValueToBoolean(cx, jsConnected, &connected);

    if (!connected) {
        JS_ReportError(cx, "The socket isn't connected.");
        JS_LeaveLocalRootScope(cx);
        JS_EndRequest(cx);
        return JS_FALSE;
    }

    char* string = (char*) JS_malloc(cx, size*sizeof(char));

    jsrefcount req = JS_SuspendRequest(cx);
    size_t offset   = 0;
    size_t received = 0;
    while (offset < size) {
        received = PR_Recv(data->socket, (string+offset), (size-offset)*sizeof(char), flags,
            (timeout == -1) ? PR_INTERVAL_NO_TIMEOUT : PR_MicrosecondsToInterval(timeout*1000000));

        if (received <= 0) {
            break;
        }
        
        offset += received;
    }
    JS_ResumeRequest(cx, req);

    if (received <= 0) {
        jsval jsConnected = JSVAL_FALSE;
        JS_SetProperty(cx, object, "connected", &jsConnected);

        PR_THROW_ERROR(cx);
        JS_LeaveLocalRootScope(cx);
        JS_EndRequest(cx);
        return JS_FALSE;
    }

    *rval = STRING_TO_JSVAL(JS_NewString(cx, string, offset));

    JS_LeaveLocalRootScope(cx);
    JS_EndRequest(cx);
    return JS_TRUE;
}


JSBool
Sockets_static_getHostByName (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval)
{
    const char* host;

    JS_BeginRequest(cx);

    if (argc != 1 || !JS_ConvertArguments(cx, argc, argv, "s", &host)) {
        JS_ReportError(cx, "Not enough paramters.");
        JS_EndRequest(cx);
        return JS_FALSE;
    }

    char* tmpHost = __Sockets_getHostByName(host);

    if (!tmpHost) {
        JS_ReportError(cx, "An error occurred while resolving the hostname.");
        JS_EndRequest(cx);
        return JS_FALSE;
    }

    *rval = STRING_TO_JSVAL(JS_NewString(cx, JS_strdup(cx, tmpHost), strlen(tmpHost)));
    delete tmpHost;

    return JS_TRUE;
}

JSBool
Sockets_static_isIPv4 (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval)
{
    const char* host;

    JS_BeginRequest(cx);
    if (argc != 1 || !JS_ConvertArguments(cx, argc, argv, "s", &host)) {
        JS_ReportError(cx, "Not enough paramters.");
        JS_EndRequest(cx);
        return JS_FALSE;
    }
    JS_EndRequest(cx);

    *rval = BOOLEAN_TO_JSVAL(__Sockets_isIPv4(host));
    return JS_TRUE;
}

