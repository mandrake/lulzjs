/* NSPR */
#define PR_GET_FD(fd) fd->secret->md.osfd

/* JAVASCRIPT */
#include "jsapi.h"
#include "jsxdrapi.h"
#include "jsfun.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
    Script (void);
    Script (JSContext* cx, const char* bytecode, uint32 length);
    Script (JSContext* cx, JSScript* script);
    Script (JSContext* cx, std::string source, std::string name = "lulzJS", int lineno = 1);
    Script (JSContext* cx, std::string path, int mode);

    virtual ~Script (void);

    bool  save (std::string path, int mode);
    bool  load (std::string path, int mode);
    void  compile (void);
    void  decompile (void);
    jsval execute (void);

    static JSScript* load (JSContext* cx, std::string path);
    static JSBool    isBytecode (const char* bytecode);
    static JSBool    isBytecode (std::ifstream* file);
    static JSBool    isCompilable (JSContext* cx, std::string source);

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
    void _stripShebang (void);
};

}
#endif
