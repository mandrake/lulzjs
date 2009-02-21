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

#include "Directory.h"

JSBool exec (JSContext* cx) { return Directory_initialize(cx); }

JSBool
Directory_initialize (JSContext* cx)
{
    JS_BeginRequest(cx);

    jsval jsParent   = JS_EVAL(cx, "System.FileSystem");
    JSObject* parent = JSVAL_TO_OBJECT(jsParent);

    JSObject* object = JS_InitClass(
        cx, parent, NULL, &Directory_class,
        Directory_constructor, 2, Directory_attributes, Directory_methods, NULL, Directory_static_methods
    );

    if (object) {
        JS_EndRequest(cx);
        return JS_TRUE;
    }

    return JS_FALSE;
}

JSBool
Directory_constructor (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval)
{
    JS_BeginRequest(cx);

    DirectoryInformation* data = new DirectoryInformation;
    data->descriptor           = NULL;
    JS_SetPrivate(cx, object, data);

    if (argc) {
        jsval ret;
        JS_CallFunctionName(cx, object, "open", argc, argv, &ret);

        if (JS_IsExceptionPending(cx)) {
            return JS_FALSE;
        }
    }

    JS_EndRequest(cx);
    return JS_TRUE;
}

void
Directory_finalize (JSContext* cx, JSObject* object)
{
    JS_BeginRequest(cx);

    DirectoryInformation* data = (DirectoryInformation*) JS_GetPrivate(cx, object);

    if (data) {
        if (data->descriptor) {
            closedir(data->descriptor);
        }

        delete data;
    }

    JS_EndRequest(cx);
}

JSBool
Directory_path_get (JSContext *cx, JSObject *obj, jsval idval, jsval *vp)
{
    DirectoryInformation* data = (DirectoryInformation*) JS_GetPrivate(cx, obj);

    JS_BeginRequest(cx);
    JS_EnterLocalRootScope(cx);

    std::cout << data->path.empty() << std::endl;

    *vp = STRING_TO_JSVAL(JS_NewString(
        cx, JS_strdup(cx, data->path.c_str()), data->path.length()
    ));

    JS_LeaveLocalRootScope(cx);
    JS_EndRequest(cx);
    return JS_TRUE;
}

JSBool
Directory_length_get (JSContext *cx, JSObject *obj, jsval idval, jsval *vp)
{

    return JS_TRUE;
}

JSBool
Directory_open (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval)
{
    char* path;

    JS_BeginRequest(cx);

    if (argc != 1 || !JS_ConvertArguments(cx, argc, argv, "s", &path)) {
        JS_ReportError(cx, "Not enough parameters.");
        return JS_FALSE;
    }

    DirectoryInformation* data = (DirectoryInformation*) JS_GetPrivate(cx, object);

    if (data->descriptor) {
        JS_ReportError(cx, "A directory is already opened.");
        return JS_FALSE;
    }

    data->path = path;

    if (stat(data->path.c_str(), &data->desc)) {
        JS_ReportError(cx, "The directory couldn't be found.");
        return JS_FALSE;
    }

    if (!S_ISDIR(data->desc.st_mode)) {
        JS_ReportError(cx, "The path doesn't lead to a directory.");
        return JS_FALSE;
    }

    data->pointers.all         = 0;
    data->pointers.files       = 0;
    data->pointers.directories = 0;

    data->descriptor = opendir(data->path.c_str());
    JS_SetPrivate(cx, object, data);

    if (!data->descriptor) {
        JS_ReportError(cx, "An error occurred while opening the directory.");
        return JS_FALSE;
    }

    return JS_TRUE;
}

JSBool
Directory_close (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval)
{
    JS_BeginRequest(cx);

    DirectoryInformation* data = (DirectoryInformation*) JS_GetPrivate(cx, object);

    if (data->descriptor) {
        closedir(data->descriptor);
        data->descriptor = NULL;
    }

    JS_EndRequest(cx);
    return JS_TRUE;
}

JSBool
Directory_readNext (JSContext *cx, JSObject *object, uintN argc, jsval *argv, jsval *rval)
{
    int lookFor = SEARCH_EVERYTHING;

    if (argc) {
        if (JS_OBJECT_IS(cx, argv[0], "System.FileSystem.File")) {
            lookFor = SEARCH_FILES;
        }
        if (JS_OBJECT_IS(cx, argv[0], "System.FileSystem.Directory")) {
            lookFor = SEARCH_DIRECTORIES;
        }
    }

    DirectoryInformation* data = (DirectoryInformation*) JS_GetPrivate(cx, object);

    switch (lookFor) {
        case SEARCH_EVERYTHING:
        seekdir(data->descriptor, data->pointers.all);
        break;

        case SEARCH_FILES:
        seekdir(data->descriptor, data->pointers.files);
        break;

        case SEARCH_DIRECTORIES:
        seekdir(data->descriptor, data->pointers.directories);
        break;
    }

    struct dirent* dir;
    while ((dir = readdir(data->descriptor))) {
        std::string name = dir->d_name;
        if (name == "." || name == "..") {
            continue;
        }

        if (lookFor == SEARCH_EVERYTHING) {
            if (dir->d_type == DT_REG || dir->d_type == DT_DIR) {
                data->pointers.all = telldir(data->descriptor);
            }
            break;
        }
        else if (lookFor == SEARCH_FILES && dir->d_type == DT_REG) {
            data->pointers.files = telldir(data->descriptor);
            break;
        }
        else if (lookFor == SEARCH_DIRECTORIES && dir->d_type == DT_DIR) {
            data->pointers.directories = telldir(data->descriptor);
            break;
        }
    }

    if (!dir) {
        *rval = JSVAL_NULL;
        return JS_TRUE;
    }

    JS_BeginRequest(cx);
    JS_EnterLocalRootScope(cx);

    std::string newPath = data->path + dir->d_name;
    jsval newArgv[]     = {
        STRING_TO_JSVAL(JS_NewString(cx, JS_strdup(cx, newPath.c_str()), newPath.length()))
    };

    std::cout << JS_GetStringBytes(JSVAL_TO_STRING(newArgv[0])) << std::endl;
    
    switch (dir->d_type) {
        case DT_REG:
        *rval = OBJECT_TO_JSVAL(JS_New(
            cx, JSVAL_TO_OBJECT(JS_EVAL(cx, "System.FileSystem.File")),
            1, newArgv
        ));
        break;

        case DT_DIR:
        *rval = OBJECT_TO_JSVAL(JS_New(
            cx, JSVAL_TO_OBJECT(JS_EVAL(cx, "System.FileSystem.Directory")),
            1, newArgv
        ));
        break;
    }

    JS_LeaveLocalRootScope(cx);
    JS_EndRequest(cx);
    return JS_TRUE;
}

JSBool
Directory_readPrev (JSContext *cx, JSObject *object, uintN argc, jsval *argv, jsval *rval)
{
    int lookFor = SEARCH_EVERYTHING;

    if (argc) {
        if (JS_OBJECT_IS(cx, argv[0], "System.FileSystem.File")) {
            lookFor = SEARCH_FILES;
        }
        if (JS_OBJECT_IS(cx, argv[0], "System.FileSystem.Directory")) {
            lookFor = SEARCH_DIRECTORIES;
        }
    }

    DirectoryInformation* data = (DirectoryInformation*) JS_GetPrivate(cx, object);

    switch (lookFor) {
        case SEARCH_EVERYTHING:
        seekdir(data->descriptor, data->pointers.all);
        break;

        case SEARCH_FILES:
        seekdir(data->descriptor, data->pointers.files);
        break;

        case SEARCH_DIRECTORIES:
        seekdir(data->descriptor, data->pointers.directories);
        break;
    }

    struct dirent* dir;
    while ((dir = readdir(data->descriptor))) {
        std::string name = dir->d_name;
        if (name == "." || name == "..") {
            continue;
        }

        if (lookFor == SEARCH_EVERYTHING) {
            data->pointers.all = telldir(data->descriptor);
            break;
        }
        else if (lookFor == SEARCH_FILES && dir->d_type == DT_REG) {
            data->pointers.files = telldir(data->descriptor);
            break;
        }
        else if (lookFor == SEARCH_DIRECTORIES && dir->d_type == DT_DIR) {
            data->pointers.directories = telldir(data->descriptor);
            break;
        }
    }

    if (!dir) {
        *rval = JSVAL_NULL;
        return JS_TRUE;
    }

    JS_BeginRequest(cx);
    JS_EnterLocalRootScope(cx);

    std::string newPath = data->path + std::string(dir->d_name);
    jsval newArgv[]     = {
        STRING_TO_JSVAL(JS_NewString(cx, JS_strdup(cx, newPath.c_str()), newPath.size()))
    };
    
    switch (dir->d_type) {
        case DT_REG:
        *rval = OBJECT_TO_JSVAL(JS_New(
            cx, JSVAL_TO_OBJECT(JS_EVAL(cx, "System.FileSystem.File")),
            1, newArgv
        ));
        break;

        case DT_DIR:
        *rval = OBJECT_TO_JSVAL(JS_New(
            cx, JSVAL_TO_OBJECT(JS_EVAL(cx, "System.FileSystem.Directory")),
            1, newArgv
        ));
        break;
    }

    JS_LeaveLocalRootScope(cx);
    JS_EndRequest(cx);
    return JS_TRUE;
}

