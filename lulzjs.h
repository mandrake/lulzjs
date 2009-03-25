#ifndef TRACEMONKEY_LULZJS_H
#define TRACEMONKEY_LULZJS_H

/* NSPR */
#include "private/prpriv.h"
#include "prerror.h"
#define PR_GET_FD(fd) PR_FileDesc2NativeHandle(fd)
#define PR_THROW_ERROR(cx) char* error = new char[PR_GetErrorTextLength()]; PR_GetErrorText(error); JS_ReportError(cx, error); delete error;

/* JAVASCRIPT */
#include "jsapi.h"
#include "jsxdrapi.h"
#include "jsfun.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <limits.h>
#include <time.h>

#include <sys/stat.h>
#include <sys/types.h>

#ifdef __cplusplus
JS_BEGIN_EXTERN_C
#endif

extern JS_PUBLIC_API(JSBool) js_CallFunctionWithNew (JSContext* cx, jsval obj, uintN argc, jsval *argv, JSObject** newObj);
#define JS_CallFunctionWithNew(cx, obj, argc, argv, newObj) js_CallFunctionWithNew(cx, obj, argc, argv, newObj)

extern JS_PUBLIC_API(JSBool) js_ObjectIs (JSContext* cx, jsval check, const char* name);
#define JS_OBJECT_IS(cx, check, name) js_ObjectIs(cx, check, name)

extern JS_PUBLIC_API(jsint) js_parseInt (JSContext* cx, jsval number, int base);
#define JS_ParseInt(cx, number, base) js_parseInt(cx, number, base)

extern JS_PUBLIC_API(jsdouble) js_parseFloat (JSContext* cx, jsval number);
#define JS_ParseFloat(cx, number, base) js_parseInt(cx, number)

extern JS_PUBLIC_API(jsval) js_eval (JSContext* cx, const char* string);
#define JS_EVAL(cx, string) js_eval(cx, string)

extern JS_PUBLIC_API(char*) js_strndup (JSContext* cx, const char* string, size_t n);
#define JS_strndup(cx, string, n) js_strndup(cx, string, n)

typedef struct {
    char*  bytecode;
    uint32 length;
} CompiledScript;

extern JS_PUBLIC_API(JSBool) Compile_stringIsBytecode (const char* bytecode);
extern JS_PUBLIC_API(JSBool) Compile_fileIsBytecode (const char* path);
extern JS_PUBLIC_API(jsval) Compile_execute (JSContext* cx, CompiledScript* compiled);
extern JS_PUBLIC_API(JSScript*) Compile_load (JSContext* cx, const char* path);
extern JS_PUBLIC_API(JSScript*) Compile_decompile (JSContext* cx, CompiledScript* compiled);
extern JS_PUBLIC_API(CompiledScript*) Compile_compile (JSContext* cx, JSScript* script);
extern JS_PUBLIC_API(CompiledScript*) Compile_compileString (JSContext* cx, const char* source);
extern JS_PUBLIC_API(JSBool) Compile_save (JSContext* cx, JSScript* script, const char* path);

#ifdef __cplusplus
JS_END_EXTERN_C

#include <iostream>
#include <string>
#include <fstream>
#include <stdexcept>

namespace lulzJS {

class Script
{
  public:
    static const int Bytecode = 0x02;
    static const int Text     = 0x04;

  public:
    Script (void)
    {
        _cx         = NULL;
        _bytecode   = NULL;
        _length     = 0;
        _script     = NULL;
        _loaded     = false;
        _compiled   = false;
        _executable = false;
    };

    Script (JSContext* cx, const char* bytecode, uint32 length)
    {
        _cx         = cx;
        _bytecode   = strdup(bytecode);
        _length     = length;
        _script     = NULL;
        _loaded     = true;
        _compiled   = true;
        _executable = false;
    };

    Script (JSContext* cx, JSScript* script)
    {
        _cx         = cx;
        _bytecode   = NULL;
        _length     = 0;
        _script     = JS_NewScriptObject(cx, script);
        _loaded     = true;
        _compiled   = false;
        _executable = true;
    };

    Script (JSContext* cx, std::string source, std::string name = "lulzJS", int lineno = 1)
    {
        _cx         = cx;
        _bytecode   = NULL;
        _length     = 0;
        _source     = source;
        _stripShebang();
    
        JS_BeginRequest(_cx);
        JSScript* script = JS_CompileScript(cx, JS_GetGlobalObject(cx), _source.c_str(), _source.length(), name.c_str(), lineno);
        
        if (script) {
            _script = JS_NewScriptObject(cx, script);
            JS_AddRoot(cx, &_script);
        }
        JS_EndRequest(_cx);
    
        if (!script) {
            throw std::runtime_error("The script couldn't be compiled.");
        }
    
        _loaded     = true;
        _compiled   = false;
        _executable = true;
    };

    Script (JSContext* cx, std::string path, int mode)
    {
        _cx         = cx;
        _bytecode   = NULL;
        _length     = 0;
        _script     = NULL;
        _loaded     = false;
        _compiled   = false;
        _executable = false;
    
        this->load(path, mode);
    };

    virtual ~Script (void)
    {
        if (_bytecode) {
            free(_bytecode);
        }
    
        if (_script) {
            JS_BeginRequest(_cx);
            JS_RemoveRoot(_cx, &_script);
            JS_EndRequest(_cx);
        }
    };

    bool  save (std::string path, int mode)
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
    };

    bool  load (std::string path, int mode)
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
                    JSScript* script = JS_CompileScript(_cx, JS_GetGlobalObject(_cx), _source.c_str(), _source.length(), path.c_str(), 1);
                    
                    if (script) {
                        _script = JS_NewScriptObject(_cx, script);
                        JS_AddRoot(_cx, &_script);
                    }
                    JS_EndRequest(_cx);
                
                    if (!script) {
                        throw std::runtime_error("The script couldn't be compiled.");
                    }
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
    };

    void  compile (void)
    {
        if (_compiled) {
            return;
        }
    
        JS_BeginRequest(_cx);
        JS_EnterLocalRootScope(_cx);
    
        JSXDRState* xdr = JS_XDRNewMem(_cx, JSXDR_ENCODE);
        char* bytes;
    
        JSScript* script = (JSScript*) JS_GetPrivate(_cx, _script);
        if (JS_XDRScript(xdr, &script)) {
            bytes     = (char*) JS_XDRMemGetData(xdr, &_length);
            _bytecode = (char*) malloc(_length*sizeof(char));
            memcpy(_bytecode, bytes, _length);
            _compiled = true;
        }
        JS_XDRDestroy(xdr);
    
        JS_LeaveLocalRootScope(_cx);
        JS_EndRequest(_cx);
    };

    void  decompile (void)
    {
        JS_BeginRequest(_cx);
        JS_EnterLocalRootScope(_cx);
    
        JSXDRState* xdr = JS_XDRNewMem(_cx, JSXDR_DECODE);
        JS_XDRMemSetData(xdr, _bytecode, _length);
    
        JSScript* script = (JSScript*) JS_GetPrivate(_cx, _script);
        if (JS_XDRScript(xdr, &script)) {
            JS_XDRMemSetData(xdr, NULL, 0);
        }
        JS_XDRDestroy(xdr);
    
        JS_LeaveLocalRootScope(_cx);
        JS_EndRequest(_cx);
    };

    jsval execute (void)
    {
        JS_BeginRequest(_cx);
    
        if (!_executable) {
            if (_compiled) {
                this->decompile();
            }
            else if (!_source.empty()) {
                JSScript* script = JS_CompileScript(_cx, JS_GetGlobalObject(_cx), _source.c_str(), _source.length(), "lulzJS", 1);
                if (script) {
                    _script = JS_NewScriptObject(_cx, script);
                    JS_AddRoot(_cx, &_script);
                }
            
                if (!script) {
                    throw std::runtime_error("The script couldn't be compiled.");
                }
                _executable = true;
            }
        }
    
        if (!_script) {
            return JSVAL_VOID;
        }
        
        jsval ret = JSVAL_VOID;
        JS_ExecuteScript(_cx, JS_GetGlobalObject(_cx), (JSScript*) JS_GetPrivate(_cx, _script), &ret);
    
        JS_EndRequest(_cx);
        return ret;
    };

    static JSBool isBytecode (const char* bytecode)
    {
        char magic[9]      = {0};
        char magicCheck[9] = {0};
        sprintf(magic,      "%8x", JSXDR_MAGIC_SCRIPT_CURRENT);
        sprintf(magicCheck, "%8x", *((unsigned int*) bytecode));
    
        return strcmp(magic, magicCheck) == 0;
    };

    static JSBool isBytecode (std::ifstream* file)
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
    };

    static JSBool isCompilable (JSContext* cx, std::string source)
    {
        JS_BeginRequest(cx);
        JSBool result = JS_BufferIsCompilableUnit(cx, JS_GetGlobalObject(cx), source.c_str(), source.length());
        JS_EndRequest(cx);
    
        return result;
    };

  private:
    JSContext*  _cx;
    char*       _bytecode;
    uint32      _length;
    JSObject*   _script;
    std::string _filename;
    std::string _source;

    bool _loaded;
    bool _compiled;
    bool _executable;

  private:
    void _stripShebang (void)
    {
        size_t eol = _source.find_first_of("\n");
    
        if (_source[0] == '#' && _source.find_first_of("!") < eol) {
            _source = _source.substr(eol+1);
        }
    };
};

}
#endif

#endif
