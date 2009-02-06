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

const char* __Core_getScriptName (JSContext* cx);
std::string __Core_getRootPath (JSContext* cx, std::string& fileName);
std::string __Core_getPath (JSContext* cx, std::string&fileName);
JSBool      __Core_include (JSContext* cx, std::string& path);
JSBool      __Core_isIncluded (const char* path);

JSObject*
Core_initialize (JSContext *cx, const char* script)
{
    JSObject* object = JS_NewObject(cx, &Core_class, NULL, NULL);

    if (object && JS_InitStandardClasses(cx, object)) {
        JS_DefineFunctions(cx, object, Core_methods);

        // Properties
        jsval property;

        std::string rootPath = __Core_getRootPath(cx, script);
        jsval paths[] = {
            STRING_TO_JSVAL(JS_NewString(cx, rootPath.c_str(), rootPath.length())),
            STRING_TO_JSVAL(JS_NewString(cx, __LJS_LIBRARY_PATH__, strlen(__LJS_LIBRARY_PATH__)))
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

        if (__Core_include(cx, __LJS_LIBRARY_PATH__ "/Core"))
            return object;
    }

    return NULL;
}

JSBool
Core_include (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
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

            char* path = __Core_getPath(cx, JS_GetStringBytes(JS_ValueToString(cx, OBJECT_TO_JSVAL(fileName))));
            jsval ret = BOOLEAN_TO_JSVAL(__Core_include(cx, path));

            JS_SetElement(cx, retArray, i, &ret);
            JS_free(cx, path);
        }

        *rval = OBJECT_TO_JSVAL(retArray);
    }
    else {
        char* path = __Core_getPath(cx, JS_GetStringBytes(JS_ValueToString(cx, OBJECT_TO_JSVAL(files))));
        *rval = BOOLEAN_TO_JSVAL(__Core_include(cx, path));
        JS_free(cx, path);
    }

    return JS_TRUE;
}

JSBool
Core_require (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    JSObject* files;

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

            char* path = __Core_getPath(cx, JS_GetStringBytes(JS_ValueToString(cx, OBJECT_TO_JSVAL(fileName))));
            if (!__Core_include(cx, path)) {
                JS_ReportError(cx, "%s couldn't be included.", path);
                JS_free(cx, path);

                return JS_FALSE;
            }
            JS_free(cx, path);
        }
    }
    else {
        char* path = __Core_getPath(cx, JS_GetStringBytes(JS_ValueToString(cx, OBJECT_TO_JSVAL(files))));
        ok = __Core_include(cx, path);

        if (!ok) {
            JS_ReportError(cx, "%s couldn't be included.", path);
            JS_free(cx, path);

            return JS_FALSE;
        }
        JS_free(cx, path);
    }

    return JS_TRUE;
}

JSBool
Core_GC (JSContext* cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    JS_MaybeGC(cx);

    return JS_TRUE;
}

JSBool
Core_die (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    char* error;

    if (argc) {
        error = JS_GetStringBytes(JSVAL_TO_STRING(argv[0]));
    }
    else {
        error = JS_strdup(cx, "Program died.");
    }

    JS_ReportError(cx, error);
    JS_ReportPendingException(cx);

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
        output = JS_realloc(cx, output, length+=512);
        read   = fread(output+(length-512), sizeof(char), 512, pipe);

        if (read < 512) {
            output = JS_realloc(cx, output, length-=(512-read));
            break;
        }
    }
    output[length-1] = '\0';
    pclose(pipe);

    *rval = STRING_TO_JSVAL(JS_NewString(cx, output, length));
    return JS_TRUE;
}

JSBool
Core_sleep (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    jsdouble time;

    if (argc != 1 || !JS_ConvertArguments(cx, argc, argv, "d", &time)) {
        JS_ReportError(cx, "Not enough parameters.");
        return JS_FALSE;
    }

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
    uintN i;
    for (i = 0; i < argc; i++) {
        printf("%s", JS_GetStringBytes(JS_ValueToString(cx, argv[i])));
    }
    puts("");

    return JS_TRUE;
}

std::string
__Core_getRootPath (JSContext* cx, std::string& fileName)
{
    std::string path;

    if (fileName == NULL) {
        path = getenv("PWD");
    }
    else if (fileName[0] == '/') {
        path = dirname(fileName);
    }
    else {
        path = dirname(getenv("PWD") + "/" + fileName);
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
__Core_getPath (JSContext* cx, std::string& fileName)
{
    /*
     * Getting the dirname of the file from the other file is included
     * then copying it and getting the path to the dir.
     */
    char* scriptName = __Core_getScriptName(cx);
    char* from       = (scriptName ? strdup(scriptName) : "");
    char* dir        = dirname(from); free(from);
    std::string path = dir + "/" + fileName;
    
    if (!fileExists(path)) {
        jsval jsPath;
        JS_GetProperty(cx, JS_GetGlobalObject(cx), "__PATH__", &jsPath);
        JSObject* lPath = JSVAL_TO_OBJECT(jsPath);

        jsuint length;
        JS_GetArrayLength(cx, lPath, &length);

        size_t i;
        for (i = 0; i < length; i++) {
            jsval pathFile;
            JS_GetElement(cx, lPath, i, &pathFile);

            path = JS_GetStringBytes(JSVAL_TO_STRING(pathFile)) + "/" + fileName;

            if (fileExists(path)) {
                break;
            }
        }
    }

    return path;
}

JSBool
__Core_include (JSContext* cx, const char* path)
{
    if (__Core_isIncluded(path)) {
        #ifdef DEBUG
        printf("(already included) %s\n", path);
        #endif

        return JS_TRUE;
    }

    if (strstr(path, ".js") == &path[strlen(path)-3]) {
        struct stat pathStat;

        if (stat(path, &pathStat) == -1) {
            #ifdef DEBUG
            printf("(javascript) %s not found.\n", path);
            #endif
            return JS_FALSE;
        }
        
        size_t length = strlen(path);

        char* cachePath = JS_malloc(cx, (length+2)*sizeof(char));
        memset(cachePath, 0, length+2); strcpy(cachePath, path);
        cachePath[length] = 'c';

        struct stat cacheStat;
        if (!stat(cachePath, &cacheStat)) {
            if (cacheStat.st_mtime >= pathStat.st_mtime) {
                jsval ret;
                JSScript* script = Compile_load(cx, cachePath);

                if (script) {
                    JS_ExecuteScript(cx, JS_GetGlobalObject(cx), script, &ret);
                    JS_DestroyScript(cx, script);

                    #ifdef DEBUG
                    printf("(cached) path: %sc\n", path);
                    #endif

                    goto exceptions;
                }
            }
        }

        #ifdef DEBUG
        printf("(javascript) path: %s\n", path);
        #endif

        jsval rval;
        char* sources    = (char*)stripRemainder(cx, (char*)readFile(cx, path));
        JSScript* script = JS_CompileScript(cx, JS_GetGlobalObject(cx), sources, strlen(sources), path, 1);
        JS_free(cx, sources);

        if (script) {
            if (JS_ExecuteScript(cx, JS_GetGlobalObject(cx), script, &rval)) {
                Compile_save(cx, script, cachePath);
            }
            JS_DestroyScript(cx, script);
        }

        exceptions:
        while (JS_IsExceptionPending(cx)) {
            JS_ReportPendingException(cx);

            return JS_FALSE;
        }
    }
    else if (strstr(path, ".so") == &path[strlen(path)-3]) {
        #ifdef DEBUG
        printf("(object) path: %s\n", path);
        #endif

        if (!fileExists(path)) {
            #ifdef DEBUG
            printf("(object) %s not found.\n", path);
            #endif
            return JS_FALSE;
        }

        void* handle = dlopen(path, RTLD_LAZY|RTLD_GLOBAL);
        char* error = dlerror();

        if (error) {
            fprintf(stderr, "%s\n", error);
            return JS_FALSE;
        }

        JSBool (*exec)(JSContext*) = dlsym(handle, "exec");

        if(!(*exec)(cx)) {
            fprintf(stderr, "The initialization of the module failed.\n");
            return JS_FALSE;
        }
    }
    else {
        #ifdef DEBUG
        printf("(module) path: %s\n", path);
        #endif

        char* newPath = JS_strdup(cx, path);
        newPath = JS_realloc(cx, newPath, (strlen(newPath)+strlen("/init.js")+1)*sizeof(char));
        strcat(newPath, "/init.js");

        if (!__Core_include(cx, newPath)) {
            JS_free(cx, newPath);
            return JS_FALSE;
        }
        JS_free(cx, newPath);
    }

    included = JS_realloc(cx, included, ++includedNumber*sizeof(char*));
    included[includedNumber-1] = JS_strdup(cx, path);

    return JS_TRUE;
}

JSBool
__Core_isIncluded (const char* path)
{
    size_t i;
    for (i = 0; i < includedNumber; i++) {
        if (strcmp(included[i], path) == 0) {
            return JS_TRUE;
        }
    }

    return JS_FALSE;
}

