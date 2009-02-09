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

#include "Core.h"

std::list<std::string> included;

const char* __Core_getScriptName (JSContext* cx);
std::string __Core_getRootPath (JSContext* cx, std::string fileName);
std::string __Core_getPath (JSContext* cx, std::string fileName);
JSBool      __Core_include (JSContext* cx, std::string path);
JSBool      __Core_isIncluded (std::string path);

JSObject*
Core_initialize (JSContext *cx, const char* script)
{
    JS_BeginRequest(cx);
    JS_SetVersion(cx, JS_StringToVersion("1.8"));

    JSObject* object = JS_NewObject(cx, &Core_class, NULL, NULL);

    if (object && JS_InitStandardClasses(cx, object)) {
        JS_DefineFunctions(cx, object, Core_methods);

        JS_EnterLocalRootScope(cx);

        // Properties
        jsval property;

        std::string rootPath = __Core_getRootPath(cx, script);
        jsval paths[] = {
            STRING_TO_JSVAL(JS_NewString(cx, JS_strdup(cx, rootPath.c_str()), rootPath.length())),
            STRING_TO_JSVAL(JS_NewString(cx, JS_strdup(cx, __LJS_LIBRARY_PATH__), strlen(__LJS_LIBRARY_PATH__)))
        };
        JSObject* path = JS_NewArrayObject(cx, 2, paths);
        property       = OBJECT_TO_JSVAL(path);
        JS_SetProperty(cx, object, "__PATH__", &property);

        property = STRING_TO_JSVAL(JS_NewString(cx, JS_strdup(cx, __LJS_VERSION__), strlen(__LJS_VERSION__)));
        JS_SetProperty(cx, object, "__VERSION__", &property);

        property = OBJECT_TO_JSVAL(object);
        JS_SetProperty(cx, object, "Program", &property);

        property = STRING_TO_JSVAL(JS_NewString(cx, JS_strdup(cx, script), strlen(script)));
        JS_SetProperty(cx, object, "name", &property);

        JS_LeaveLocalRootScope(cx);
        JS_EndRequest(cx);

        if (__Core_include(cx, __LJS_LIBRARY_PATH__ "/Core"))
            return object;
    }

    return NULL;
}

JSBool
Core_include (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    JS_BeginRequest(cx);

    JSObject* files;

    if (argc != 1 || !JS_ConvertArguments(cx, argc, argv, "o", &files)) {
        JS_ReportError(cx, "You must pass only the modules/files to include.");
        return JS_FALSE;
    }

    if (JS_IsArrayObject(cx, files)) {
        JSObject* retArray = JS_NewArrayObject(cx, 0, NULL);

        jsuint length;
        JS_GetArrayLength(cx, files, &length);

        jsuint i;
        for (i = 0; i < length; i++) {
            jsval fileName;
            JS_GetElement(cx, files, i, &fileName);

            std::string path = __Core_getPath(cx, JS_GetStringBytes(JS_ValueToString(cx, fileName)));
            jsval ret = BOOLEAN_TO_JSVAL(__Core_include(cx, path));

            JS_SetElement(cx, retArray, i, &ret);
        }

        *rval = OBJECT_TO_JSVAL(retArray);
    }
    else {
        std::string path = __Core_getPath(cx, JS_GetStringBytes(JS_ValueToString(cx, OBJECT_TO_JSVAL(files))));
        *rval = BOOLEAN_TO_JSVAL(__Core_include(cx, path));
    }

    JS_EndRequest(cx);

    return JS_TRUE;
}

JSBool
Core_require (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    JSObject* files;

    JS_BeginRequest(cx);

    if (argc != 1 || !JS_ConvertArguments(cx, argc, argv, "o", &files)) {
        JS_ReportError(cx, "You must pass only the modules/files to include.");
        return JS_FALSE;
    }

    short ok;

    if (JS_IsArrayObject(cx, files)) {
        jsuint length;
        JS_GetArrayLength(cx, files, &length);

        size_t i;
        for (i = 0; i < length; i++) {
            jsval fileName;
            JS_GetElement(cx, files, i, &fileName);

            std::string path = __Core_getPath(cx, JS_GetStringBytes(JS_ValueToString(cx, fileName)));
            if (!__Core_include(cx, path)) {
                JS_ReportError(cx, "%s couldn't be included.", path.c_str());

                return JS_FALSE;
            }
        }
    }
    else {
        std::string path = __Core_getPath(cx, JS_GetStringBytes(JS_ValueToString(cx, OBJECT_TO_JSVAL(files))));
        ok = __Core_include(cx, path);

        if (!ok) {
            JS_ReportError(cx, "%s couldn't be included.", path.c_str());

            return JS_FALSE;
        }
    }

    JS_EndRequest(cx);

    return JS_TRUE;
}

JSBool
Core_GC (JSContext* cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    JS_BeginRequest(cx);
    JS_MaybeGC(cx);
    JS_EndRequest(cx);

    return JS_TRUE;
}

JSBool
Core_die (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    char* error;

    JS_BeginRequest(cx);

    if (argc) {
        error = JS_GetStringBytes(JS_ValueToString(cx, argv[0]));
    }
    else {
        error = strdup("Program died.");
    }

    JS_ReportError(cx, error);
    JS_ReportPendingException(cx);

    JS_EndRequest(cx);

    exit(EXIT_FAILURE);
    return JS_FALSE;
}

JSBool
Core_exit (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    exit(EXIT_SUCCESS);
    return JS_FALSE;
}

JSBool
Core_ENV (JSContext* cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    JS_BeginRequest(cx);

    if (argc < 1) {
        JS_ReportError(cx, "Not enough parameters.");
        return JS_FALSE;
    }

    char* env = JS_GetStringBytes(JS_ValueToString(cx, argv[0]));

    if (argc == 1) {
        char* envValue = getenv(env);

        *rval = (envValue != NULL)
            ? STRING_TO_JSVAL(JS_NewString(cx, JS_strdup(cx, envValue), strlen(envValue)))
            : JSVAL_NULL;
    }
    else {
        if (JSVAL_IS_NULL(argv[1])) {
            unsetenv(env);
            *rval = JSVAL_NULL;
        }
        else {
            char* value = JS_GetStringBytes(JS_ValueToString(cx, argv[1]));
            *rval = BOOLEAN_TO_JSVAL(!setenv(env, value, 1));
        }
    }

    JS_EndRequest(cx);

    return JS_TRUE;
}

JSBool
Core_exec (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    FILE* pipe;
    char* output  = NULL;
    size_t length = 0;
    size_t read   = 0;
    const char* command;

    JS_BeginRequest(cx);
    JS_EnterLocalRootScope(cx);

    if (argc != 1 || !JS_ConvertArguments(cx, argc, argv, "s", &command)) {
        JS_ReportError(cx, "Not enough parameters.");
        return JS_FALSE;
    }
    
    if ((pipe = popen(command, "r")) == NULL) {
        JS_ReportError(cx, "Command not found");
        return JS_FALSE;
    }

    // Read untill the pipe is empty.
    while (1) {
        output = (char*) JS_realloc(cx, output, length+=512);
        read   = fread(output+(length-512), sizeof(char), 512, pipe);

        if (read < 512) {
            output = (char*) JS_realloc(cx, output, length-=(512-read));
            break;
        }
    }
    output[length-1] = '\0';
    pclose(pipe);

    *rval = STRING_TO_JSVAL(JS_NewString(cx, output, length));

    JS_LeaveLocalRootScope(cx);
    JS_EndRequest(cx);

    return JS_TRUE;
}

JSBool
Core_sleep (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    jsdouble time;

    JS_BeginRequest(cx);

    if (argc != 1 || !JS_ConvertArguments(cx, argc, argv, "d", &time)) {
        JS_ReportError(cx, "Not enough parameters.");
        return JS_FALSE;
    }

    JS_EndRequest(cx);

    if (time < 1) {
        usleep(time * 1000000);
    }
    else {
        sleep(time);
    }

    return JS_TRUE;
}

JSBool
Core_print (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    JS_BeginRequest(cx);

    uintN i;
    for (i = 0; i < argc; i++) {
        printf("%s", JS_GetStringBytes(JS_ValueToString(cx, argv[i])));
    }
    puts("");

    JS_EndRequest(cx);

    return JS_TRUE;
}

std::string
__Core_getRootPath (JSContext* cx, std::string fileName)
{
    std::string path;

    if (fileName.empty()) {
        path = getenv("PWD");
    }
    else if (fileName[0] == '/') {
        char* tmp = strdup(fileName.c_str());
        path      = dirname(tmp);
        free(tmp);
    }
    else {
        char* tmp = strdup((std::string(getenv("PWD")) + "/" + fileName).c_str());
        path      = dirname(tmp);
        free(tmp);
    }

    return path;
}

const char*
__Core_getScriptName (JSContext* cx)
{
    JSStackFrame* fp = NULL;
    fp = JS_FrameIterator(cx, &fp); fp = JS_FrameIterator(cx, &fp);
    JSScript* script = JS_GetFrameScript(cx, fp);

    return JS_GetScriptFilename(cx, script);
}

std::string
__Core_getPath (JSContext* cx, std::string fileName)
{
    /*
     * Getting the dirname of the file from the other file is included
     * then copying it and getting the path to the dir.
     */
    const char* scriptName = __Core_getScriptName(cx);
    char* from             = strdup(scriptName ? scriptName : "");
    char* dir              = dirname(from);
    std::string path       = std::string(dir) + "/" + fileName;
    free(from);

    JS_BeginRequest(cx);
    
    struct stat test;
    if (stat(path.c_str(), &test) != 0) {
        jsval jsPath;
        JS_GetProperty(cx, JS_GetGlobalObject(cx), "__PATH__", &jsPath);
        JSObject* lPath = JSVAL_TO_OBJECT(jsPath);

        jsuint length;
        JS_GetArrayLength(cx, lPath, &length);

        size_t i;
        for (i = 0; i < length; i++) {
            jsval pathFile;
            JS_GetElement(cx, lPath, i, &pathFile);

            path = std::string(JS_GetStringBytes(JSVAL_TO_STRING(pathFile))) + "/" + fileName;

            if (stat(path.c_str(), &test) == 0) {
                break;
            }
        }
    }

    JS_EndRequest(cx);

    return path;
}

JSBool
__Core_include (JSContext* cx, std::string path)
{
    if (__Core_isIncluded(path)) {
        #ifdef DEBUG
        std::cerr << "(already included) " << path << "." << std::endl;
        #endif

        return JS_TRUE;
    }

    if (path.substr(path.length()-3) == ".js") {
        struct stat pathStat;

        if (stat(path.c_str(), &pathStat) == -1) {
            #ifdef DEBUG
            std::cerr << "(javascript) " << path << "not found." << std::endl;
            #endif
            return JS_FALSE;
        }
        
        std::string cachePath = path + "c";
        struct stat cacheStat;
        if (!stat(cachePath.c_str(), &cacheStat)) {
            if (cacheStat.st_mtime >= pathStat.st_mtime) {
                lulzJS::Script script(cx, cachePath, lulzJS::Script::Bytecode);
                script.execute();

                if (JS_IsExceptionPending(cx)) {
                    JS_ReportPendingException(cx);
                    return JS_FALSE;
                }
            }
        }

        #ifdef DEBUG
        std::cerr << "(javascript) path: " << path << std::endl;
        #endif

        lulzJS::Script script(cx, path, lulzJS::Script::Text);
        script.execute();

        if (!JS_IsExceptionPending(cx)) {
            script.save(cachePath, lulzJS::Script::Bytecode);
        }
        else {
            JS_ReportPendingException(cx);
            return JS_FALSE;
        }
    }
    else if (path.substr(path.length()-3) == ".so") {
        #ifdef DEBUG
        std::cerr << "(object) path: " << path << std::endl;
        #endif

        struct stat test;
        if (stat(path.c_str(), &test)) {
            #ifdef DEBUG
            std::cerr << "(object) " << path << " not found." << std::endl;
            #endif
            return JS_FALSE;
        }

        void* handle      = dlopen(path.c_str(), RTLD_LAZY|RTLD_GLOBAL);
        const char* error = dlerror();

        if (error) {
            std::cerr << error << std::endl;
            return JS_FALSE;
        }

        JSBool (*exec)(JSContext*) = (JSBool (*)(JSContext*)) dlsym(handle, "exec");

        if (exec == NULL || !(*exec)(cx)) {
            std::cerr << "The initialization of the module failed." << std::endl;
            return JS_FALSE;
        }
    }
    else {
        #ifdef DEBUG
        std::cerr << "(module) path: " << path << std::endl;
        #endif

        if (!__Core_include(cx, path + "/init.js")) {
            return JS_FALSE;
        }
    }

    included.push_back(path);

    return JS_TRUE;
}

JSBool
__Core_isIncluded (std::string path)
{
    std::list<std::string>::iterator i;

    for (i = included.begin(); i != included.end(); i++) {
        if (*i == path) {
            return JS_TRUE;
        }
    }

    return JS_FALSE;
}

