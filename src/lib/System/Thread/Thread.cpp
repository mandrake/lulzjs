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

#include "Thread.h"

void* __Thread_start (void* arg);

JSBool exec (JSContext* cx) { return Thread_initialize(cx); }

void
reportError (JSContext *cx, const char *message, JSErrorReport *report)
{
    fprintf(stderr, "%s:%u > %s\n",
        "Thread",
        (unsigned int) report->lineno,
        message
    );
}

JSBool
Thread_initialize (JSContext* cx)
{
    JS_BeginRequest(cx);
    
    JSObject* parent = JS_GetGlobalObject(cx);

    JSObject* object = JS_InitClass(
        cx, parent, NULL, &Thread_class,
        Thread_constructor, 1, NULL, Thread_methods, NULL, Thread_static_methods
    );

    if (object) {
        JS_EndRequest(cx);
        return JS_TRUE;
    }

    return JS_FALSE;
}

JSBool
Thread_constructor (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval)
{
    jsval klass;
    jsval detach = JSVAL_FALSE;

    JS_BeginRequest(cx);

    if (argc < 1) {
        JS_ReportError(cx, "Thread needs an object to transform into a thread.");
        return JS_FALSE;
    }

    switch (argc) {
        case 2: detach = argv[1]; break;
        case 1: klass  = argv[0];  break;
    }

    pthread_t* thread =  new pthread_t;
    JS_SetPrivate(cx, object, thread);

    JS_SetProperty(cx, object, "__class", &klass);
    JS_SetProperty(cx, object, "__detach", &detach);

    JS_EndRequest(cx);

    return JS_TRUE;
}

void
Thread_finalize (JSContext* cx, JSObject* object)
{
    JS_BeginRequest(cx);

    pthread_t* thread = (pthread_t*) JS_GetPrivate(cx, object);

    if (thread) {
        delete thread;
    }

    JS_EndRequest(cx);
}

JSBool
Thread_start (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval)
{
    JS_BeginRequest(cx);

    pthread_t* thread = (pthread_t*) JS_GetPrivate(cx, object);

    ThreadData* data = new ThreadData;
    data->cx = JS_NewContext(JS_GetRuntime(cx), 8192);
    JS_SetOptions(data->cx, JSOPTION_VAROBJFIX);
    JS_SetErrorReporter(data->cx, reportError);
    JS_SetGlobalObject(data->cx, JS_GetGlobalObject(cx));

    data->object = object;
    data->argc   = argc;
    data->argv   = new jsval[argc];;
    memcpy(data->argv, argv, argc*sizeof(jsval*));

    jsval property; JS_GetProperty(cx, object, "__detach", &property);
    JSBool detach = JSVAL_TO_BOOLEAN(property);

    #if !defined(DEBUG)
    if (detach) {
        JS_ReportError(cx, "Detached threads still have some problems.");
        return JS_FALSE;
    }
    #endif

    pthread_create(thread, NULL, __Thread_start, data);

    char id[26]; memset(id, 0, 26);
    sprintf(id, "%lu", pthread_self());
    property = STRING_TO_JSVAL(JS_NewString(cx, JS_strdup(cx, id), strlen(id)));
    JS_SetProperty(cx, object, "__id", &property);

    // TODO: Make this work.
    if (detach) {
        pthread_detach(*thread);
    }

    JS_EndRequest(cx);

    return JS_TRUE;
}

void*
__Thread_start (void* arg)
{
    ThreadData* data  = (ThreadData*) arg;
    JSContext* cx     = data->cx;
    JSObject*  object = data->object;
    uintN      argc   = data->argc;
    jsval*     argv   = data->argv;
    delete data;

    JS_SetContextThread(cx);
    JS_BeginRequest(cx);
    jsval property; JS_GetProperty(cx, object, "__detach", &property);

    JSBool detach = JSVAL_TO_BOOLEAN(property);

    JS_GetProperty(cx, object, "__going", &property);

    if (!JSVAL_IS_VOID(property) && JSVAL_TO_BOOLEAN(property)) {
        return NULL;
    }

    property = JSVAL_TRUE; JS_SetProperty(cx, object, "__going", &property);
    property = JSVAL_TRUE; JS_SetProperty(cx, object, "__started", &property);

    // Get the class that's the actual class to construct.
    JS_GetProperty(cx, object, "__class", &property);
    JSObject* klass = JSVAL_TO_OBJECT(property);

    // Get the prototype of the object to use in the JS_ConstructObject
    jsval jsProto; JS_GetProperty(cx, klass, "prototype", &jsProto);
    JSObject* proto = JSVAL_TO_OBJECT(jsProto);

    // Construct the object if it's a javascript thingy.
    JSObject* threadObj = JS_ConstructObject(cx, NULL, proto, NULL);
    property = OBJECT_TO_JSVAL(threadObj);
    JS_SetProperty(cx, object, "__object", &property);

    // Execute the actual javascript constructor
    jsval ret;
    JS_CallFunctionValue(cx, threadObj, OBJECT_TO_JSVAL(klass), argc, argv, &ret);

    property = JSVAL_FALSE;
    JS_SetProperty(cx, object, "__going", &property);

    delete argv;
    JS_EndRequest(cx);
    JS_ClearContextThread(cx);
    JS_DestroyContext(cx);

    if (!detach) {
        pthread_exit(NULL);
    }

    return NULL;
}

JSBool
Thread_join (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval)
{
    JS_BeginRequest(cx);

    jsval detach; JS_GetProperty(cx, object, "__detach", &detach);
    if (JSVAL_TO_BOOLEAN(detach)) {
        JS_ReportError(cx, "You can't join a detached thread.");
        return JS_FALSE;
    }

    pthread_t* thread = (pthread_t*) JS_GetPrivate(cx, object);
    pthread_join(*thread, NULL);

    jsval ret;
    JS_GetProperty(cx, object, "__object", &ret);
    JS_GetProperty(cx, JSVAL_TO_OBJECT(ret), "__return", &ret);

    *rval = ret;

    JS_EndRequest(cx);
    return JS_TRUE;
}

JSBool
Thread_stop (JSContext *cx, JSObject *object, uintN argc, jsval *argv, jsval *rval)
{
    JS_BeginRequest(cx);

    pthread_t* thread = (pthread_t*) JS_GetPrivate(cx, object);

    jsval property = JSVAL_FALSE;
    JS_SetProperty(cx, object, "__going", &property);

    *rval = BOOLEAN_TO_JSVAL(pthread_cancel(*thread));

    JS_EndRequest(cx);
    return JS_TRUE;
}

JSBool
Thread_static_cancel (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval)
{
    JSObject* obj;

    JS_BeginRequest(cx);

    if (argc != 1 || !JS_ConvertArguments(cx, argc, argv, "o", &obj)) {
        JS_ReportError(cx, "Not enough paramteres.");
        return JS_FALSE;
    }

    pthread_t *thread = (pthread_t*) JS_GetInstancePrivate(cx, obj, &Thread_class, NULL);

    if (!thread) {
        JS_ReportError(cx, "It's not a thread.");
        return JS_FALSE;
    }

    *rval = BOOLEAN_TO_JSVAL(pthread_cancel(*thread));

    JS_EndRequest(cx);

    return JS_TRUE;
}

JSBool
Thread_static_cancelPoint (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval)
{
    pthread_testcancel();
    return JS_TRUE;
}

