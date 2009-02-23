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

#ifndef _JS_LULJS_H
#define _JS_LULJS_H

#include "jsapi.h"
#include "jsxdrapi.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <time.h>

#include <sys/stat.h>
#include <sys/types.h>

extern "C" {

JSObject* js_CallFunctionWithNew (JSContext* cx, JSObject* obj, uintN argc, jsval *argv);
#define JS_CallFunctionWithNew(cx, obj, argc, argv) js_CallFunctionWithNew(cx, obj, argc, argv)

JSBool js_ObjectIs (JSContext* cx, jsval check, const char* name);
#define JS_OBJECT_IS(cx, check, name) js_ObjectIs(cx, check, name)

jsint js_parseInt (JSContext* cx, jsval number, int base);
#define JS_ParseInt(cx, number, base) js_parseInt(cx, number, base)

jsdouble js_parseFloat (JSContext* cx, jsval number);
#define JS_ParseFloat(cx, number, base) js_parseInt(cx, number)

jsval js_eval (JSContext* cx, const char* string);
#define JS_EVAL(cx, string) js_eval(cx, string)

typedef struct {
    char*  bytecode;
    uint32 length;
} CompiledScript;

JSBool          Compile_stringIsBytecode (const char* bytecode);
JSBool          Compile_fileIsBytecode (const char* path);
jsval           Compile_execute (JSContext* cx, CompiledScript* compiled);
JSScript*       Compile_load (JSContext* cx, const char* path);
JSScript*       Compile_decompile (JSContext* cx, CompiledScript* compiled);
CompiledScript* Compile_compile (JSContext* cx, JSScript* script);
CompiledScript* Compile_compileString (JSContext* cx, const char* source);
JSBool          Compile_save (JSContext* cx, JSScript* script, const char* path);

}

#ifdef __cplusplus
#include <iostream>
#include <string>
#include <fstream>

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
    Script (JSContext* cx, std::string source);
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

  private:
    JSContext*  _cx;
    char*       _bytecode;
    uint32      _length;
    JSScript*   _script;
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

#endif
