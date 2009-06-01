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

void
reportError (JSContext *cx, const char *message, JSErrorReport *report)
{
    FCGX_Request* cgi = (FCGX_Request*) JS_GetContextPrivate(cx);

    FCGX_FPrintF(cgi->err, "%s:%u > %s\n",
        report->filename ? report->filename : "lulzJS",
        (unsigned int) report->lineno,
        message
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
            JS_SetContextPrivate(engine.context, cgi);
            JS_SetErrorReporter(engine.context, reportError);

            struct stat check;
            if (stat(__LJS_LIBRARY_PATH__"/Core/Core.so", &check) != 0) {
                return engine;
            }

            PRLibrary* lib = PR_LoadLibrary(__LJS_LIBRARY_PATH__"/Core/Core.so");
            JSBool (*coreInitialize)(JSContext*, const char*)
                = (JSBool (*)(JSContext*, const char*)) PR_FindSymbol(lib, "Core_initialize");

            if (coreInitialize == NULL || !(*coreInitialize)(engine.context, getenv("SCRIPT_NAME"))) {
                return engine;
            }

            engine.core = JS_GetGlobalObject(engine.context);
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

            property = STRING_TO_JSVAL(
                JS_NewString(engine.context, JS_strdup(engine.context, name.c_str()), name.length())
            );
            JS_DefineProperty(engine.context, engine.core, "name", property, NULL, NULL, JSPROP_READONLY);

            property = STRING_TO_JSVAL(
                JS_NewString(engine.context, JS_strdup(engine.context, path.c_str()), path.length())
            );
            JS_DefineProperty(engine.context, engine.core, "path", property, NULL, NULL, JSPROP_READONLY);

            property = INT_TO_JSVAL(getpid());
            JS_DefineProperty(engine.context, engine.core, "PID", property, NULL, NULL, JSPROP_READONLY);

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

