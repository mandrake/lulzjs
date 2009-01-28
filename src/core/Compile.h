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

JSBool    Compile_isBytecode (const char* bytecode);
JSBool    Compile_execute (JSContext* cx, const char* bytecode);
JSScript* Compile_load (JSContext* cx, const char* path);
JSScript* Compile_decompile (JSContext* cx, const char* bytecode);
char*     Compile_compile (JSContext* cx, JSScript* script);
JSBool    Compile_save (JSContext* cx, JSScript* script, const char* path);

#endif
