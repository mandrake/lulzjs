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

#include "Compile.h"

JSBool
Compile_stringIsBytecode (const char* bytecode)
{
    char magic[9]      = {0};
    char magicCheck[9] = {0};
    sprintf(magic,      "%8x", JSXDR_MAGIC_SCRIPT_CURRENT);
    sprintf(magicCheck, "%8x", *((unsigned long*) bytecode));

    return strcmp(magic, magicCheck) == 0;
}

JSBool
Compile_fileIsBytecode (const char* path)
{
    struct stat fileStat;
     
    if (!stat(path, &fileStat)) {
        if (fileStat.st_size > 3) {
            FILE* file = fopen(path, "rb");

            char bytecode[4];
            fread(bytecode, sizeof(char), 4, file);
            fclose(file);

            return Compile_stringIsBytecode(bytecode);
        }
    }

    return JS_FALSE;
}


JSBool
Compile_execute (JSContext* cx, CompiledScript* compiled)
{
    JSScript* script = Compile_decompile(cx, compiled);
    JSBool result = JS_FALSE;

    if (script) {
        jsval ret;
        result = JS_ExecuteScript(cx, JS_GetGlobalObject(cx), script, &ret);
        JS_DestroyScript(cx, script);
    }
    
    return result;
}

JSScript*
Compile_load (JSContext* cx, const char* path)
{
    FILE* cache;
    JSScript* script;
    char* buffer;
    struct stat cacheStat;

    if (!(cache = fopen(path, "rb"))) {
        return NULL;
    }
    stat(path, &cacheStat);

    buffer = readFile(cx, path);

    CompiledScript compiled;
    compiled.bytecode = buffer;
    compiled.length   = cacheStat.st_size;
    script            = Compile_decompile(cx, &compiled);

    fclose(cache);
    JS_free(cx, buffer);

    return script;
}

JSScript*
Compile_decompile (JSContext* cx, CompiledScript* compiled)
{
    JSScript* script = NULL;

    if (compiled->length > 3 && Compile_stringIsBytecode(compiled->bytecode)) {
        JSXDRState* xdr = JS_XDRNewMem(cx, JSXDR_DECODE);
        JS_XDRMemSetData(xdr, compiled->bytecode, compiled->length);

        if (JS_XDRScript(xdr, &script)) {
            JS_XDRMemSetData(xdr, NULL, 0);
            JS_XDRDestroy(xdr);
        }
    }

    return script;
}

CompiledScript*
Compile_compile (JSContext* cx, JSScript* script)
{
    JSXDRState* xdr = JS_XDRNewMem(cx, JSXDR_ENCODE);
    CompiledScript* compiled = JS_malloc(cx, sizeof(CompiledScript));
    char* bytes;

    if (JS_XDRScript(xdr, &script)) {
        bytes = JS_XDRMemGetData(xdr, &compiled->length);
        compiled->bytecode = JS_malloc(cx, compiled->length*sizeof(char));
        memcpy(compiled->bytecode, bytes, compiled->length);
    }
    else {
        compiled->bytecode = NULL;
        compiled->length   = 0;
    }
    JS_XDRDestroy(xdr);

    return compiled;
}

CompiledScript*
Compile_compileString (JSContext* cx, const char* source)
{
    JSScript* script         = JS_CompileScript(cx, JS_GetGlobalObject(cx), source, strlen(source), NULL, 1);
    CompiledScript* compiled = Compile_compile(cx, script);
    JS_DestroyScript(cx, script);
    return compiled;
}

JSBool
Compile_save (JSContext* cx, JSScript* script, const char* path)
{
    FILE* file;

    if (!(file = fopen(path, "wb"))) {
        return JS_FALSE;
    }

    CompiledScript* compiled = Compile_compile(cx, script);

    uint32 offset = 0;
    while (offset < compiled->length) {
        offset += fwrite((compiled->bytecode+offset), sizeof(char), (compiled->length-offset), file);
    }
    fclose(file);
    JS_free(cx, compiled->bytecode);
    JS_free(cx, compiled);

    return JS_TRUE;
}

