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

#include "lulzjs.h"
#include "fcgiapp.h"
#include "prthread.h"

#include "common.h"
#include "Headers.h"
#include "Output.h"
#include "Input.h"

// *nix only
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

static JSRuntime* LJS_runtime = NULL;

typedef struct {
    JSBool     error;
    JSRuntime* runtime;
    JSContext* context;
    JSObject*  core;
} Engine;

JSBool
print (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    JS_BeginRequest(cx);
    JS_EnterLocalRootScope(cx);

    JSObject* global = ((LCGIData*) JS_GetContextPrivate(cx))->global;

    char*        separator = " ";
    char*        end       = "\n";
    jsval        property;
    JSObject*    options;

    if (argc > 1 && JS_TypeOfValue(cx, argv[argc-1]) == JSTYPE_OBJECT) {
        JS_ValueToObject(cx, argv[argc-1], &options);
        argc--;

        JS_GetProperty(cx, options, "separator", &property);
        if (JSVAL_IS_VOID(property) || JSVAL_IS_NULL(property)) {
            JS_GetProperty(cx, options, "sep", &property);
        }

        if (JSVAL_IS_STRING(property)) {
            separator = JS_GetStringBytes(JS_ValueToString(cx, property));
        }

        JS_GetProperty(cx, options, "end", &property);
        if (JSVAL_IS_STRING(property)) {
            end = JS_GetStringBytes(JS_ValueToString(cx, property));
        }
    }

    char* string    = NULL;
    char* tmpString = NULL;
    int   length    = 0;
    int   tmpLength = 0;

    uintN i;
    for (i = 0; i < argc; i++) {
        tmpString = JS_GetStringBytes(JS_ValueToString(cx, argv[i]));
        tmpLength = strlen(tmpString);
        string    = (char*) JS_realloc(cx, string, (length+tmpLength+1)*sizeof(char));
        strcat(string+length, tmpString);
        length += tmpLength;

        if (i != argc-1) {
            tmpLength = strlen(separator);
            string    = (char*) JS_realloc(cx, string, (length+tmpLength+1)*sizeof(char));
            strcat(string+length, separator);
            length += tmpLength;
        }
    }
    tmpLength = strlen(end);
    string    = (char*) JS_realloc(cx, string, (length+tmpLength+1)*sizeof(char));
    strcat(string+length, end);
    length += tmpLength;

    jsval propery;
    JS_GetProperty(cx, global, "Output", &property);
    JSObject* Output = JSVAL_TO_OBJECT(property);

    jsval args[] = { STRING_TO_JSVAL(JS_NewString(cx, string, length)) };
    JS_CallFunctionName(cx, Output, "write", 1, args, rval);

    JS_LeaveLocalRootScope(cx);
    JS_EndRequest(cx);

    return JS_TRUE;
}

void
reportError (JSContext *cx, const char *message, JSErrorReport *report)
{
    LCGIData* data = (LCGIData*) JS_GetContextPrivate(cx);

    jsval property;
    JS_GetProperty(cx, data->global, "Output", &property);
    JSObject* Output = JSVAL_TO_OBJECT(property);
    JS_CallFunctionName(cx, Output, "flush", 0, NULL, &property);

    FCGX_Request* cgi = data->cgi;

    FCGX_FPrintF(cgi->err, "<p>%s <i>in</i> <b>%s</b> <i>at line</i> <b>%u</b></p>\n",
        message,
        report->filename ? report->filename : "lulzJS",
        (unsigned int) report->lineno
    );
}

Engine
initEngine (FCGX_Request* cgi)
{
    Engine engine;
    engine.error = JS_TRUE;

    JS_SetCStringsAreUTF8();

    if ((engine.runtime = LJS_runtime)) {
        if ((engine.context = JS_NewContext(engine.runtime, 8192))) {
            JS_BeginRequest(engine.context);
            JS_EnterLocalRootScope(engine.context);
            JS_SetErrorReporter(engine.context, reportError);

            struct stat check;
            if (stat(__LJS_LIBRARY_PATH__"/Core/Core.so", &check) != 0) {
                return engine;
            }

            PRLibrary* lib = PR_LoadLibrary(__LJS_LIBRARY_PATH__"/Core/Core.so");
            JSBool (*coreInitialize)(JSContext*, const char*)
                = (JSBool (*)(JSContext*, const char*)) PR_FindSymbol(lib, "Core_initialize");

            if (coreInitialize == NULL || !(*coreInitialize)(engine.context, getenv("PATH_TRANSLATED"))) {
                return engine;
            }

            engine.core = JS_GetGlobalObject(engine.context);

            LCGIData* data = new LCGIData;
            data->cgi      = cgi;
            data->global   = engine.core;
            JS_SetContextPrivate(engine.context, data);

            jsval property;
            
            std::string full = getenv("SCRIPT_NAME");
            std::string name;
            std::string path;

            size_t slashPosition = full.find_last_of("/\\");
            if (slashPosition != std::string::npos) {
                name = full.substr(slashPosition+1);
                path = full.substr(0, slashPosition+1);
            }
            else {
                name = full;
                path = "./";
            }

            property = STRING_TO_JSVAL(JS_NewStringCopyZ(engine.context, name.c_str()));
            JS_DefineProperty(engine.context, engine.core, "name", property, NULL, NULL, JSPROP_READONLY);
            property = STRING_TO_JSVAL(JS_NewStringCopyZ(engine.context, path.c_str()));
            JS_DefineProperty(engine.context, engine.core, "path", property, NULL, NULL, JSPROP_READONLY);

            JS_LeaveLocalRootScope(engine.context);
            JS_EndRequest(engine.context);

            #ifdef GCBOOM
            JS_SetGCZeal(engine.context, 2);
            #endif

            engine.error = JS_FALSE;
            return engine;
        }
    }

    return engine;
}

void
executeScript (FCGX_Request* cgi)
{
    Engine engine = initEngine(cgi);
    if (engine.error) {
        FCGX_PutS("Couldn't initialize the engine.\n", cgi->err);
        FCGX_Finish_r(cgi);
        return;
    }

    Headers_initialize(engine.context);
    Input_initialize(engine.context);
    Output_initialize(engine.context);

    // Do the rest here

    FCGX_Finish_r(cgi);
}

int
main (int argc, char *argv[])
{
    int error;
    PRThread*     thread = NULL;
    FCGX_Request* cgi    = NULL;

    if ((error = FCGX_Init())) {
        fprintf(stderr, "FastCGI got borked: %d\n", error);
        return 1;
    }

    if ((LJS_runtime = JS_NewRuntime(8L * 1024L * 1024L)) == NULL) {
        fprintf(stderr, "Error in creating the JSRuntime.\n");
        return -1;
    }

    if (FCGX_IsCGI()) {
        cgi   = new FCGX_Request;
        error = FCGX_InitRequest(cgi, 0, 0);

        if (error) {
            fprintf(stderr, "FastCGI got borked: %d\n", error);
            return 2;
        }

        error = FCGX_Accept_r(cgi);

        if (error) {
            fprintf(stderr, "FastCGI got borked: %d\n", error);
            return 3;
        }

        executeScript(cgi);
    }
    else {
        while (true) {
            cgi   = new FCGX_Request;
            error = FCGX_InitRequest(cgi, 0, 0);

            if (error) {
                fprintf(stderr, "FastCGI got borked: %d\n", error);
                return 2;
            }

            error = FCGX_Accept_r(cgi);

            if (error) {
                fprintf(stderr, "FastCGI got borked: %d\n", error);
                return 3;
            }

            thread = PR_CreateThread(
                PR_USER_THREAD,
                (void (*)(void*)) executeScript, cgi,
                PR_PRIORITY_HIGH, PR_LOCAL_THREAD, PR_UNJOINABLE_THREAD, 0
            );
        }
    }

    FCGX_ShutdownPending();

    return EXIT_SUCCESS;
}

