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

JSBool
js_CallFunctionWithNew (JSContext* cx, jsval obj, uintN argc, jsval *argv, JSObject** newObject)
{
    jsval property;

    JS_BeginRequest(cx);
    JS_EnterLocalRootScope(cx);

    JSObject* classObj; JS_ValueToObject(cx, obj, &classObj);

    if (!classObj) {
        return JS_FALSE;
    }

    JSClass* klass = JS_GET_CLASS(cx, classObj);
    *newObject     = JS_NewObject(cx, klass, NULL, NULL);

    if (!*newObject) {
        return JS_FALSE;
    }

    if (!JS_GetProperty(cx, classObj, "prototype", &property)) {
        return JS_FALSE;
    }

    if (!JSVAL_IS_VOID(property)) {
        if (!JS_SetPrototype(cx, *newObject, JSVAL_TO_OBJECT(property))) {
            return JS_FALSE;
        }
    }

    if (!JS_CallFunctionValue(cx, *newObject, obj, argc, argv, &property)) {
        return JS_FALSE;
    }

    if (!JSVAL_IS_VOID(property)) {
        *newObject = JSVAL_TO_OBJECT(property);
    }

    JS_LeaveLocalRootScope(cx);
    JS_EndRequest(cx);

    return JS_TRUE;
}

JSBool
js_ObjectIs (JSContext* cx, jsval check, const char* name)
{
    JS_BeginRequest(cx);
    JS_EnterLocalRootScope(cx);

    jsval jsObj; JS_GetProperty(cx, JS_GetGlobalObject(cx), "Object", &jsObj);
    JSObject* Obj = JSVAL_TO_OBJECT(jsObj);

    jsval newArgv[] = {
        check,
        STRING_TO_JSVAL(JS_NewString(cx, JS_strdup(cx, name), strlen(name)))
    };
    jsval ret; JS_CallFunctionName(cx, Obj, "is", 2, newArgv, &ret);

    JS_LeaveLocalRootScope(cx);
    JS_EndRequest(cx);
    return JSVAL_TO_BOOLEAN(ret);
}

jsint
js_parseInt (JSContext* cx, jsval number, int base)
{
    JS_BeginRequest(cx);
    JS_EnterLocalRootScope(cx);

    jsval ret;

    if (base >= 2 && base <= 36) {
        jsval argv[] = {number, INT_TO_JSVAL(base)};
        JS_CallFunctionName(cx, JS_GetGlobalObject(cx), "parseInt", 2, argv, &ret);
    }
    else {
        jsval argv[] = {number};
        JS_CallFunctionName(cx, JS_GetGlobalObject(cx), "parseInt", 1, argv, &ret);
    }

    if (strcmp(JS_GetStringBytes(JS_ValueToString(cx, ret)), "NaN") == 0) {
        return 0;
    }

    jsint nret; JS_ValueToInt32(cx, ret, &nret);

    JS_LeaveLocalRootScope(cx);
    JS_EndRequest(cx);
    return nret;
}

jsdouble
js_parseFloat (JSContext* cx, jsval number)
{
    JS_BeginRequest(cx);
    JS_EnterLocalRootScope(cx);

    jsval ret;

    jsval argv[] = {number};
    JS_CallFunctionName(cx, JS_GetGlobalObject(cx), "parseFloat", 1, argv, &ret);

    if (strcmp(JS_GetStringBytes(JS_ValueToString(cx, ret)), "NaN") == 0) {
        return 0;
    }
    
    jsdouble nret; JS_ValueToNumber(cx, ret, &nret);

    JS_LeaveLocalRootScope(cx);
    JS_EndRequest(cx);
    return nret;
}

jsval
js_eval (JSContext* cx, const char* string)
{
    JS_BeginRequest(cx);
    JS_EnterLocalRootScope(cx);

    jsval ret;

    JS_EvaluateScript(cx, JS_GetGlobalObject(cx), string, strlen(string), "eval", 1, &ret);

    if (JS_IsExceptionPending(cx)) {
        JS_ClearPendingException(cx);
        ret = JSVAL_VOID;
    }

    JS_LeaveLocalRootScope(cx);
    JS_EndRequest(cx);
    return ret;
}

JSBool
Compile_stringIsBytecode (const char* bytecode)
{
    char magic[9]      = {0};
    char magicCheck[9] = {0};
    sprintf(magic,      "%8x", JSXDR_MAGIC_SCRIPT_CURRENT);
    sprintf(magicCheck, "%8x", *((unsigned int*) bytecode));

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


jsval
Compile_execute (JSContext* cx, CompiledScript* compiled)
{
    JSScript* script = Compile_decompile(cx, compiled);

    jsval ret = JSVAL_VOID;
    if (script) {
        JS_ExecuteScript(cx, JS_GetGlobalObject(cx), script, &ret);
        JS_DestroyScript(cx, script);
    }
    
    return ret;
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

    buffer = (char*) malloc(cacheStat.st_size*sizeof(char));
    fwrite(buffer, sizeof(char), cacheStat.st_size, cache);

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
    CompiledScript* compiled = (CompiledScript*) malloc(sizeof(CompiledScript));
    char* bytes;

    if (JS_XDRScript(xdr, &script)) {
        bytes = (char*) JS_XDRMemGetData(xdr, &compiled->length);
        compiled->bytecode = (char*) malloc(compiled->length*sizeof(char));
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

namespace lulzJS {

Script::Script (void)
{
    _cx         = NULL;
    _bytecode   = NULL;
    _length     = 0;
    _script     = NULL;
    _loaded     = false;
    _compiled   = false;
    _executable = false;
}

Script::Script (JSContext* cx, const char* bytecode, uint32 length)
{
    _cx         = cx;
    _bytecode   = strdup(bytecode);
    _length     = length;
    _script     = NULL;
    _loaded     = true;
    _compiled   = true;
    _executable = false;
}

Script::Script (JSContext* cx, JSScript* script)
{
    _cx         = cx;
    _bytecode   = NULL;
    _length     = 0;
    _script     = script;
    _loaded     = true;
    _compiled   = false;
    _executable = true;
}

Script::Script (JSContext* cx, std::string source)
{
    _cx         = cx;
    _bytecode   = NULL;
    _length     = 0;
    _source     = source;
    _stripShebang();
    JS_BeginRequest(_cx);
    _script     = JS_CompileScript(cx, JS_GetGlobalObject(cx), _source.c_str(), _source.length(), "lulzJS", 1);
    JS_AddRoot(_cx, _script);
    JS_EndRequest(_cx);
    _loaded     = true;
    _compiled   = false;
    _executable = true;
}

Script::Script (JSContext* cx, std::string path, int mode)
{
    _cx         = cx;
    _bytecode   = NULL;
    _length     = 0;
    _script     = NULL;
    _loaded     = false;
    _compiled   = false;
    _executable = false;

    this->load(path, mode);
}

Script::~Script (void)
{
    if (_bytecode) {
        free(_bytecode);
    }

    if (_script) {
        JS_BeginRequest(_cx);
        JS_RemoveRoot(_cx, _script);
        JS_DestroyScript(_cx, _script);
        JS_EndRequest(_cx);
    }
}

bool
Script::save (std::string path, int mode)
{
    switch (mode) {
        case Text: {
            return false;
        } break;

        case Bytecode: {
            if (!_compiled) {
                return false;
            }

            std::ofstream file(path.c_str(), std::ios_base::binary|std::ios_base::out);
            if (file.is_open()) {
                file.write(_bytecode, _length);
                file.close();

                return true;
            }
            
            return false;
        } break;
    }

    return false;
}

bool
Script::load (std::string path, int mode)
{
    if (_loaded) {
        return false;
    }

    _filename = path;

    switch (mode) {
        case Text: {
            struct stat fileStat;
            if (!stat(path.c_str(), &fileStat)) {
                std::ifstream file(path.c_str());

                char* source = new char[fileStat.st_size+1];
                file.read(source, fileStat.st_size);
                source[fileStat.st_size] = '\0';
                _source = source; _stripShebang();
                JS_BeginRequest(_cx);
                _script = JS_CompileScript(_cx, JS_GetGlobalObject(_cx), _source.c_str(), _source.length(), path.c_str(), 1);
                JS_EndRequest(_cx);
                delete [] source;

                file.close();

                _loaded     = true;
                _executable = true;
                return true;
            }

            return false;
        } break;

        case Bytecode: {
            struct stat fileStat;
            if (!stat(path.c_str(), &fileStat)) {
                std::ifstream file(path.c_str(), std::ios_base::binary|std::ios_base::in);
        
                _length   = fileStat.st_size;
                _bytecode = (char*) malloc(_length*sizeof(char));
                file.read(_bytecode, _length);
                file.close();

                _loaded   = true;
                _compiled = true;
                return true;
            }

            return false;
        } break;
    }

    return false;
}

void
Script::compile (void)
{
    if (_compiled) {
        return;
    }

    JS_BeginRequest(_cx);
    JS_EnterLocalRootScope(_cx);

    JSXDRState* xdr = JS_XDRNewMem(_cx, JSXDR_ENCODE);
    char* bytes;

    if (JS_XDRScript(xdr, &_script)) {
        bytes     = (char*) JS_XDRMemGetData(xdr, &_length);
        _bytecode = (char*) malloc(_length*sizeof(char));
        memcpy(_bytecode, bytes, _length);
        _compiled = true;
    }
    JS_XDRDestroy(xdr);

    JS_LeaveLocalRootScope(_cx);
    JS_EndRequest(_cx);
}

void
Script::decompile (void)
{
    JS_BeginRequest(_cx);
    JS_EnterLocalRootScope(_cx);

    JSXDRState* xdr = JS_XDRNewMem(_cx, JSXDR_DECODE);
    JS_XDRMemSetData(xdr, _bytecode, _length);

    if (JS_XDRScript(xdr, &_script)) {
        JS_XDRMemSetData(xdr, NULL, 0);
    }
    JS_XDRDestroy(xdr);

    JS_LeaveLocalRootScope(_cx);
    JS_EndRequest(_cx);
}

jsval
Script::execute (void)
{
    JS_BeginRequest(_cx);

    if (!_executable) {
        if (_compiled) {
            this->decompile();
        }
        else if (!_source.empty()) {
            _script = JS_CompileScript(_cx, JS_GetGlobalObject(_cx), _source.c_str(), _source.length(), _filename.c_str(), 1);
            JS_AddRoot(_cx, _script);
            _executable = true;
        }
    }

    if (!_script) {
        return JSVAL_VOID;
    }
    
    jsval ret = JSVAL_VOID;
    JS_ExecuteScript(_cx, JS_GetGlobalObject(_cx), _script, &ret);

    JS_EndRequest(_cx);
    return ret;
}

JSBool
Script::isBytecode (const char* bytecode)
{
    char magic[9]      = {0};
    char magicCheck[9] = {0};
    sprintf(magic,      "%8x", JSXDR_MAGIC_SCRIPT_CURRENT);
    sprintf(magicCheck, "%8x", *((unsigned int*) bytecode));

    return strcmp(magic, magicCheck) == 0;
}

JSBool
Script::isBytecode (std::ifstream* file)
{
    bool result = false;

    if (file->is_open()) {
        file->seekg(0, std::ios_base::beg);
        std::ifstream::pos_type begin_pos = file->tellg();
        file->seekg(0, std::ios_base::end);

        if ((file->tellg() - begin_pos) > 3) {
            file->seekg(0, std::ios_base::beg);
            char* bytecode = new char[4];
            file->read(bytecode, 4);
            result = Script::isBytecode(bytecode);
            delete [] bytecode;
        }
    }
    delete file;

    return result;
}

void
Script::_stripShebang (void)
{
    size_t eol = _source.find_first_of("\n");

    if (_source[0] == '#' && _source.find_first_of("!") < eol) {
        _source = _source.substr(eol+1);
    }
}

}

