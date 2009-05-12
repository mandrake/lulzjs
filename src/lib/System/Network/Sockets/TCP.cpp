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

#include "TCP.h"
#include "Sockets_common.cpp"

JSBool exec (JSContext* cx) { return TCP_initialize(cx); }

JSBool
TCP_initialize (JSContext* cx)
{
    JS_BeginRequest(cx);
    JS_EnterLocalRootScope(cx);

    JSObject* parent = JSVAL_TO_OBJECT(JS_EVAL(cx, "System.Network.Sockets"));

    JSObject* object = JS_InitClass(
        cx, parent, JSVAL_TO_OBJECT(JS_EVAL(cx, "System.Network.Sockets.prototype")), &TCP_class,
        TCP_constructor, 1, TCP_attributes, TCP_methods, NULL, TCP_static_methods
    );

    if (object) {
        JS_LeaveLocalRootScope(cx);
        JS_EndRequest(cx);
        return JS_TRUE;
    }

    return JS_FALSE;
}

JSBool
TCP_constructor (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval)
{
    TCPInformation* data = new TCPInformation;
    JS_SetPrivate(cx, object, data);
    data->socket = PR_NewTCPSocket();

    jsval jsConnected = JSVAL_FALSE;
    JS_SetProperty(cx, object, "connected", &jsConnected);

    return JS_TRUE;
}

void
TCP_finalize (JSContext* cx, JSObject* object)
{
    JS_BeginRequest(cx);

    TCPInformation* data = (TCPInformation*) JS_GetPrivate(cx, object);

    if (data) {
        delete data;
    }

    JS_EndRequest(cx);
}

JSBool
TCP_connect (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval)
{
    char*    host;
    int      port    = -1;
    jsdouble timeout = 30;

    JS_BeginRequest(cx);

    if (argc < 1) {
        JS_ReportError(cx, "Not enough parameters.");
        JS_EndRequest(cx);
        return JS_FALSE;
    }

    jsval jsConnected;
    JS_GetProperty(cx, object, "connected", &jsConnected);

    if (jsConnected == JSVAL_TRUE) {
        JS_ReportError(cx, "The socket is already connected.");
        JS_EndRequest(cx);
        return JS_FALSE;
    }

    switch (argc) {
        case 3: JS_ValueToNumber(cx, argv[2], &timeout);
        case 2: JS_ValueToInt32(cx, argv[1], &port);
        case 1: host = JS_GetStringBytes(JS_ValueToString(cx, argv[0]));
    }

    TCPInformation* data = (TCPInformation*) JS_GetPrivate(cx, object);
    PRNetAddr addr; 

    if (__Sockets_initAddr(&addr, host, port) == PR_FAILURE) {
        *rval = JSVAL_FALSE;
        JS_EndRequest(cx);
        return JS_TRUE;
    }

    if (PR_Connect(data->socket, &addr, (timeout == -1)
        ? PR_INTERVAL_NO_TIMEOUT
        : PR_MicrosecondsToInterval(timeout*1000000)) == PR_FAILURE)
    {
        if (PR_GetError() == PR_CONNECT_RESET_ERROR) {
            JS_ReportError(cx, "Connection reset by peer.");
        }

        jsConnected = JSVAL_FALSE;
        JS_SetProperty(cx, object, "connected", &jsConnected);

        JS_EndRequest(cx);
        return JS_FALSE;
    }

    jsConnected = JSVAL_TRUE;
    JS_SetProperty(cx, object, "connected", &jsConnected);

    JS_EndRequest(cx);
    return JS_TRUE;
}

JSBool
TCP_listen (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval)
{
    char* host;
    int   port    = -1;
    int   maxconn = 255;

    jsval jsConnected;

    JS_BeginRequest(cx);

    if (argc < 1) {
        JS_ReportError(cx, "Not enough parameters.");
        JS_EndRequest(cx);
        return JS_FALSE;
    }

    JS_EnterLocalRootScope(cx);

    switch (argc) {
        case 3: JS_ValueToInt32(cx, argv[2], &maxconn);
        case 2: JS_ValueToInt32(cx, argv[1], &port);
        case 1: host = JS_GetStringBytes(JS_ValueToString(cx, argv[0]));
    }

    TCPInformation* data = (TCPInformation*) JS_GetPrivate(cx, object);
    PRNetAddr addr;

    if (JSVAL_IS_NULL(argv[0]) || JSVAL_IS_VOID(argv[0])) {
        PR_InitializeNetAddr(PR_IpAddrAny, port, &addr);
    }
    else {
        __Sockets_initAddr(&addr, host, port);
    }
    JS_LeaveLocalRootScope(cx);

    int on = 1;
    setsockopt(PR_GET_FD(data->socket), SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

    jsrefcount req = JS_SuspendRequest(cx);

    if (PR_Bind(data->socket, &addr) == PR_FAILURE) {
        JS_ResumeRequest(cx, req);

        switch (PR_GetError()) {
            case PR_NO_ACCESS_RIGHTS_ERROR:
            JS_ReportError(cx, "You can't listen on this port.");
            break;

            default: JS_ReportError(cx, "Something went wrong."); break;
        }

        JS_EndRequest(cx);
        return JS_FALSE;
    }

    if (PR_Listen(data->socket, maxconn) == PR_FAILURE) {
        JS_ResumeRequest(cx, req);

        switch (PR_GetError()) {
            default: JS_ReportError(cx, "Something went wrong."); break;
        }

        JS_EndRequest(cx);
        return JS_FALSE;
    }

    JS_ResumeRequest(cx, req);

    jsConnected = JSVAL_TRUE;
    JS_SetProperty(cx, object, "connected", &jsConnected);

    JS_EndRequest(cx);
    return JS_TRUE;
}

JSBool
TCP_accept (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval)
{
    jsdouble timeout = -1;

    if (argc) {
        JS_ValueToNumber(cx, argv[0], &timeout);
    }

    TCPInformation* data = (TCPInformation*) JS_GetPrivate(cx, object);
    
    JS_BeginRequest(cx);
    JS_EnterLocalRootScope(cx);

    JSObject* sock = JS_NewObject(cx, &TCP_class, JS_GetPrototype(cx, object), NULL);
    TCPInformation* newData = new TCPInformation;
    JS_SetPrivate(cx, sock, newData);

    PRNetAddr addr;
    newData->socket = PR_Accept(data->socket, &addr, (timeout == -1)
        ? PR_INTERVAL_NO_TIMEOUT
        : PR_MicrosecondsToInterval(timeout*1000000));

    jsval property = JSVAL_TRUE;
    JS_SetProperty(cx, sock, "connected", &property);

    *rval = OBJECT_TO_JSVAL(sock);

    JS_LeaveLocalRootScope(cx);
    JS_EndRequest(cx);
    return JS_TRUE;
}

JSBool
TCP_write (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval)
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

    TCPInformation* data = (TCPInformation*) JS_GetPrivate(cx, object);

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

        offset += sent;
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
TCP_read (JSContext *cx, JSObject *object, uintN argc, jsval *argv, jsval *rval)
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

    switch (argc) {
        case 3: JS_ValueToNumber(cx, argv[2], &timeout);
        case 2: JS_ValueToUint16(cx, argv[1], &flags);
        case 1: JS_ValueToInt32(cx, argv[0], &size);
    }

    TCPInformation* data = (TCPInformation*) JS_GetPrivate(cx, object);

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
    int32 offset   = 0;
    int32 received = 0;
    while (offset < size) {
        received = PR_Recv(data->socket, (string+offset), (size-offset)*sizeof(char), flags,
            (timeout == -1) ? PR_INTERVAL_NO_TIMEOUT : PR_MicrosecondsToInterval(timeout*1000000));

        if (received == -1) {
            break;
        }
        
        offset += received;
    }
    JS_ResumeRequest(cx, req);

    if (received < 0) {
        PR_THROW_ERROR(cx);
        JS_EndRequest(cx);
        return JS_FALSE;
    }

    *rval = STRING_TO_JSVAL(JS_NewString(cx, string, offset));

    JS_EndRequest(cx);
    return JS_TRUE;
}


