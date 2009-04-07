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

#include "Sockets.h"

char*    __Sockets_getHostByName (const char* host);
JSBool   __Sockets_isIPv4 (const char* host);
PRStatus __Sockets_initAddr (PRNetAddr* addr, const char* host, int port = -1);

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
Sockets_connect (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval)
{
    char*    host;
    int      port    = -1;
    jsdouble timeout = -1;

    JS_BeginRequest(cx);

    if (argc < 1) {
        JS_ReportError(cx, "Not enough parameters.");
        JS_EndRequest(cx);
        return JS_FALSE;
    }

    switch (argc) {
        case 3: JS_ValueToNumber(cx, argv[2], &timeout);
        case 2: JS_ValueToInt32(cx, argv[1], &port);
        case 1: host = JS_GetStringBytes(JS_ValueToString(cx, argv[0]));
    }

    SocketsInformation* data = (SocketsInformation*) JS_GetPrivate(cx, object);
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
        PR_THROW_ERROR(cx);
        JS_EndRequest(cx);
    }

    jsval jsConnected = JSVAL_TRUE;
    JS_SetProperty(cx, object, "connected", &jsConnected);

    JS_EndRequest(cx);
    return JS_TRUE;
}

JSBool
Sockets_listen (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval)
{
    char* host;
    int   port    = -1;
    int   maxconn = 255;

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

    SocketsInformation* data = (SocketsInformation*) JS_GetPrivate(cx, object);
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
        PR_THROW_ERROR(cx);
        JS_EndRequest(cx);
        return JS_FALSE;
    }

    if (PR_Listen(data->socket, maxconn) == PR_FAILURE) {
        JS_ResumeRequest(cx, req);
        PR_THROW_ERROR(cx);
        JS_EndRequest(cx);
        return JS_FALSE;
    }

    JS_ResumeRequest(cx, req);
    JS_EndRequest(cx);
    return JS_TRUE;
}

JSBool
Sockets_accept (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval)
{
    jsdouble timeout = -1;

    if (argc) {
        JS_ValueToNumber(cx, argv[0], &timeout);
    }

    SocketsInformation* data = (SocketsInformation*) JS_GetPrivate(cx, object);
    
    JS_BeginRequest(cx);
    JS_EnterLocalRootScope(cx);

    JSObject* sock = JS_NewObject(cx, &Sockets_class, JS_GetPrototype(cx, object), NULL);
    SocketsInformation* newData = new SocketsInformation;
    JS_SetPrivate(cx, sock, newData);

    PRNetAddr addr;
    newData->socket = PR_Accept(data->socket, &addr, (timeout == -1)
        ? PR_INTERVAL_NO_TIMEOUT
        : PR_MicrosecondsToInterval(timeout*1000000));

    *rval = OBJECT_TO_JSVAL(sock);

    JS_LeaveLocalRootScope(cx);
    JS_EndRequest(cx);
    return JS_TRUE;
}

JSBool
Sockets_write (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval)
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
Sockets_read (JSContext *cx, JSObject *object, uintN argc, jsval *argv, jsval *rval)
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

char*
__Sockets_getHostByName (const char* host)
{
    char* buf = new char[PR_NETDB_BUF_SIZE];
    PRHostEnt hp; 
    if (PR_GetHostByName(host, buf, PR_NETDB_BUF_SIZE, &hp) == PR_FAILURE) {
        return NULL;
    }
    delete [] buf;

    char* ip = new char[INET6_ADDRSTRLEN];
    int offset = 0;

    offset += sprintf(&ip[offset], "%u.", (unsigned char) hp.h_name[0]);
    offset += sprintf(&ip[offset], "%u.", (unsigned char) hp.h_name[1]);
    offset += sprintf(&ip[offset], "%u.", (unsigned char) hp.h_name[2]);
    offset += sprintf(&ip[offset], "%u",  (unsigned char) hp.h_name[3]);

    return ip;
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

JSBool
__Sockets_isIPv4 (const char* host)
{
    char klass[4];
    memset(klass, 0, 4);

    short number;
    size_t i;
    for (i = 0, number = 0; i < strlen(host); i++) {
        if (number == 3) {
            return JS_FALSE;
        }

        if (host[i] == '.') {
            number = 0;
            short part = atoi(klass);

            if (part < 0 || part > 255) {
                return JS_FALSE;
            }

            memset(klass, 0, 4);
        }

        klass[number] = host[i];
        number++;
    }

    return JS_TRUE;
}

PRStatus
__Sockets_initAddr (PRNetAddr* addr, const char* host, int port)
{
    std::string sHost = host;

    if (PR_StringToNetAddr(sHost.c_str(), addr) == PR_FAILURE) {
        std::string sIp;
        std::string sPort;

        if (sHost[0] == '[') {
            sIp   = sHost.substr(1, sHost.find_last_of("]")-1);
            sPort = sHost.substr(sHost.find_last_of("]")+2);
        }
        else {
            sIp   = sHost.substr(0, sHost.find_last_of(":"));
            sPort = sHost.substr(sHost.find_last_of(":")+1);
        }

        if (PR_StringToNetAddr(sIp.c_str(), addr) == PR_FAILURE) {
            sPort = "";
            if (sHost.find(":") != std::string::npos) {
                sHost = sHost.substr(0, sHost.find_last_of(":"));
                sPort = sHost.substr(sHost.find_last_of(":")+1);
            }

            char* ip = __Sockets_getHostByName(sHost.c_str());
            sIp      = ip; delete [] ip;

            if (PR_StringToNetAddr(sIp.c_str(), addr) == PR_FAILURE) {
                return PR_FAILURE;
            }
        }

        if (!sPort.empty()) {
            port = atoi(sPort.c_str());
        }
    }

    if (port >= 0) {
        PR_InitializeNetAddr(PR_IpAddrNull, port, addr);
    }

    return PR_SUCCESS;
}

