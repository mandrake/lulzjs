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
#include <iostream>
#include <fstream>

#include "Core.h"
#include "Interactive.h"

char USAGE[] = {
    "lulzJS " __LJS_VERSION__ "\n"
    "\n"
    "    -V        Get lulzJS version.\n"
    "    -c        Compile javascript into bytecode.\n"
    "    -o        Name of the compiled javascript.\n"
    "    -h        Read this help.\n"
};

typedef struct {
    JSBool     error;
    JSRuntime* runtime;
    JSContext* context;
    JSObject*  core;
} Engine;

void
reportError (JSContext *cx, const char *message, JSErrorReport *report)
{
    fprintf(stderr, "%s:%u > %s\n",

        report->filename ? report->filename : "lulzJS",
        (unsigned int) report->lineno,
        message
    );
}


Engine initEngine (int argc, int optind, char *argv[]);
JSBool executeScript (JSContext* cx, std::string file);

int
main (int argc, char *argv[])
{
    char* oneliner = NULL;
    int stopAt     = argc;

    JSBool compile      = JS_FALSE;
    std::string inFile  = "";
    std::string outFile = "out.jsc";

    // Fix the options to let a script get all the real arguments.
    if (argc > 1) {
        int i;
        char prev = '\0';
        for (i = 1; i < argc; i++) {
            if (argv[i][0] != '-' && (prev != 'e' && prev != 'c' && prev != 'o')) {
                stopAt = i;
                break;
            }

            if (strlen(argv[i]) >= 2) {
                if (argv[i][0] == '-') {
                    prev = argv[i][1];
                }
            }
        }
    }

    int cmd;
    while ((cmd = getopt(stopAt, argv, "vVhe:c:o:")) != -1) {
        switch (cmd) {
            case 'V':
            puts("lulzJS " __LJS_VERSION__);
            return 0;
            break;

            case 'e':
            oneliner = optarg;
            break;

            case 'c':
            compile = JS_TRUE;
            inFile  = optarg;
            break;

            case 'o':
            outFile = optarg;
            break;

            default:
            puts(USAGE);
            return 0;
        }
    }
    
    /*
     * Engine initialization, 0 because the Program.name will be ljs or the name
     * of the interpreter on the system, if instead it's executing a file it passes
     * optind because it will be the script name.
     */
    Engine engine = initEngine(argc, (argc == 1 || (oneliner || compile) ? 0 : optind), argv);
    if (engine.error) {
        std::cerr << "An error occurred while initializing the system." << std::endl;
        return 1;
    }

    #ifdef GCBOOM
    JS_SetGCZeal(engine.context, 2);
    #endif

    if (argc == 1) {
        Interactive(engine.context, engine.core);
    }
    else if (oneliner) {
        lulzJS::Script script(engine.context, oneliner);

        jsval ret = script.execute();
        if (JS_IsExceptionPending(engine.context)) {
            JS_ReportPendingException(engine.context);
            return EXIT_FAILURE;
        }

        if (!JSVAL_IS_VOID(ret)) {
            std::cout << JS_GetStringBytes(JS_ValueToString(engine.context, ret)) << std::endl;
        }
    }
    else if (compile) {
        lulzJS::Script script(engine.context, inFile, lulzJS::Script::Text);

        if (!script.save(outFile, lulzJS::Script::Bytecode)) {
            std::cerr << "Couldn't write to " << outFile << std::endl;
            return EXIT_FAILURE;
        }
    }
    else {
        struct stat exists;
        if (stat(argv[optind], &exists)) {
            std::cerr << "The file doesnt't exist." << std::endl;
            return EXIT_FAILURE;
        }
        if (!executeScript(engine.context, argv[optind])) {
            std::cerr << "The script couldn't be executed." << std::endl;
            return EXIT_FAILURE;
        }
    }
    
    JS_DestroyContext(engine.context);
    JS_DestroyRuntime(engine.runtime);
    JS_ShutDown();

    return EXIT_SUCCESS;
}


Engine
initEngine (int argc, int offset, char *argv[])
{
    Engine engine;
    engine.error = JS_TRUE;

    if ((engine.runtime = JS_NewRuntime(8L * 1024L * 1024L))) {
        if ((engine.context = JS_NewContext(engine.runtime, 8192))) {
            JS_BeginRequest(engine.context);
            JS_EnterLocalRootScope(engine.context);

            JS_SetOptions(engine.context, JSOPTION_VAROBJFIX);
            JS_SetErrorReporter(engine.context, reportError);

            if ((engine.core = Core_initialize(engine.context, argv[offset]))) {
                jsval property;
                JSObject* arguments = JS_NewArrayObject(engine.context, 0, NULL);
                property = OBJECT_TO_JSVAL(arguments);
                JS_SetProperty(engine.context, engine.core, "arguments", &property);

                if (offset+1 < argc) {
                    int i;
                    jsval rval;
                    for (i = offset+1; i < argc; i++) {
                        property = STRING_TO_JSVAL(JS_NewString(engine.context, JS_strdup(engine.context, argv[i]), strlen(argv[i])));
                        JS_CallFunctionName(
                            engine.context, arguments, "push",
                            1, &property, &rval);
                    }
                }

                JS_LeaveLocalRootScope(engine.context);
                JS_EndRequest(engine.context);

                engine.error = JS_FALSE;
                return engine;
            }
        }
    }

    return engine;
}

JSBool
executeScript (JSContext* cx, std::string file)
{
    JSBool returnValue;

    if (lulzJS::Script::isBytecode(new std::ifstream(file.c_str()))) {
        lulzJS::Script script(cx, file, lulzJS::Script::Bytecode);
        returnValue = script.execute();
    }
    else {
        lulzJS::Script script(cx, file, lulzJS::Script::Text);
        returnValue = script.execute();
    }

    if (JS_IsExceptionPending(cx)) {
        JS_ReportPendingException(cx);
    }

    return returnValue;
}

