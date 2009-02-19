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
        Directory_constructor, 2, NULL, Directory_methods, NULL, Directory_static_methods
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
        free(data->path);

        if (data->directory) {
            closedir(data->directory);
        }

        delete data;
    }

    JS_EndRequest(cx);
}

JSBool
Directory_open (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval)
{
    char* path;

    JS_BeginRequest(cx);

    if (argc != 1 || !JS_ConvertArguments(cx, argc, argv, "s", &path)) {
        JS_ReportError(cx, "Directory requires the path and the mode as arguments.");
        return JS_FALSE;
    }

    DirectoryInformation* data = (DirectoryInformation*) JS_GetPrivate(cx, object);

    if (data->descriptor) {
        JS_ReportError(cx, "A directory is already opened.");
        return JS_FALSE;
    }

    data->path = strdup(path);

    if (stat(data->path, &data->desc)) {
        JS_ReportError(cx, "An error occurred while opening the file.");
        return JS_FALSE;
    }

    if (!S_ISDIR(data->desc.st_mode)) {
        JS_ReportError(cx, "The path doesn't lead to a regular file.");
        return JS_FALSE;
    }

    data->pointers.all         = 0;
    data->pointers.files       = 0;
    data->pointers.directories = 0;

    data->descriptor = opendir(data->path);
    JS_SetPrivate(cx, object, data);

    if (!data->descriptor) {
        JS_ReportError(cx, "An error occurred while opening the directory.");
        return JS_FALSE;
    }
}

JSBool
Directory_close (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval)
{
    JS_BeginRequest(cx);

    DirectoryInformation* data = (DirectoryInformation*) JS_GetPrivate(cx, object);

    if (data->directory) {
        closedir(data->directory);
        data->directory = NULL;
        free(data->path);
    }

    JS_EndRequest(cx);
    return JS_TRUE;
}

JSBool
Directory_next (JSContext *cx, JSObject *object, uintN argc, jsval *argv, jsval *rval)
{
    JSObject* type;

    if (argc) {
        JS_ValueToObject(cx, argv[0], &type);
    }

    DirectoryInformation* data = (DirectoryInformation*) JS_GetPrivate(cx, object);

    JS_EnterLocalRootScope(cx);
    char* string = (char*) JS_malloc(cx, (size+1)*sizeof(char));
    memset(string, 0, size+1);
    fread(string, sizeof(char), size, data->descriptor);
    *rval = STRING_TO_JSVAL(JS_NewString(cx, string, size));
    JS_LeaveLocalRootScope(cx);

    return JS_TRUE;
}

