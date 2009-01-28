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
Compile_isBytecode (const char* bytecode)
{
    // Can't get the check with numbers, i don't know why
    char magic[9]      = {0};
    char magicCheck[9] = {0};
    sprintf(magic,      "%x", JSXDR_MAGIC_SCRIPT_CURRENT);
    sprintf(magicCheck, "%x", (*(unsigned long*) bytecode));

    return (strcmp(magic, magicCheck) == 0)
        ? JS_TRUE
        : JS_FALSE;
}

JSBool
Compile_execute (JSContext* cx, const char* bytecode)
{
    JSScript* script = Compile_decompile(cx, bytecode);
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

    buffer = JS_malloc(cx, (cacheStat.st_size+1)*sizeof(char));

    uint32 offset = 0;
    while (offset < cacheStat.st_size) {
        offset += fread((buffer+offset), sizeof(char), (cacheStat.st_size-offset), cache);
    }
    buffer[cacheStat.st_size] = '\0';

    script = Compile_decompile(cx, buffer);

    fclose(cache);
    JS_free(cx, buffer);

    return script;
}

JSScript*
Compile_decompile (JSContext* cx, const char* bytecode)
{
    JSScript* script = NULL;

    uint32 length = strlen(bytecode);

    if (length > 3 && Compile_isBytecode(bytecode)) {
        JSXDRState* xdr = JS_XDRNewMem(cx, JSXDR_DECODE);
        JS_XDRMemSetData(xdr, bytecode, length);

        if (JS_XDRScript(xdr, &script)) {
            JS_XDRMemSetData(xdr, NULL, 0);
            JS_XDRDestroy(xdr);
        }
    }

    return script;
}

char*
Compile_compile (JSContext* cx, JSScript* script)
{
    JSXDRState* xdr = JS_XDRNewMem(cx, JSXDR_ENCODE);
    char* bytes;

    if (JS_XDRScript(xdr, &script)) {
        uint32 length = 0;
        bytes = JS_strdup(cx, JS_XDRMemGetData(xdr, &length));
    }
    else {
        bytes = NULL;
    }
    JS_XDRDestroy(xdr);
    
    return bytes;
}

JSBool
Compile_save (JSContext* cx, JSScript* script, const char* path)
{
    FILE* file;

    if (!(file = fopen(path, "wb"))) {
        return JS_FALSE;
    }

    char* bytecode = Compile_compile(cx, script);
    size_t length  = strlen(bytecode);

    uint32 offset = 0;
    while (offset < length) {
        offset += fwrite((bytecode+offset), sizeof(char), (length-offset), file);
    }
    JS_free(cx, bytecode);

    return JS_TRUE;
}

