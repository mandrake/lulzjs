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

#ifndef _COMPILE_H
#define _COMPILE_H

#include "lulzjs.h"
#include "jsxdrapi.h"

// Not cross platform
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

typedef struct {
    char*  bytecode;
    uint32 length;
} CompiledScript;

inline JSBool   Compile_stringIsBytecode (const char* bytecode);
JSBool          Compile_fileIsBytecode (const char* path);
JSBool          Compile_execute (JSContext* cx, CompiledScript* compiled);
JSScript*       Compile_load (JSContext* cx, const char* path);
JSScript*       Compile_decompile (JSContext* cx, CompiledScript* compiled);
CompiledScript* Compile_compile (JSContext* cx, JSScript* script);
CompiledScript* Compile_compileString (JSContext* cx, const char* source);
JSBool          Compile_save (JSContext* cx, JSScript* script, const char* path);

#endif
