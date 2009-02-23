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

    if (!data->descriptor) {
        JS_ReportError(cx, "You have to open a directory first.");
        return JS_FALSE;
    }

    JS_BeginRequest(cx);
    JS_EnterLocalRootScope(cx);

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
    DirectoryInformation* data = (DirectoryInformation*) JS_GetPrivate(cx, obj);

    *vp = INT_TO_JSVAL(data->pointers.size());

    return JS_TRUE;
}

JSBool
Directory_position_get (JSContext *cx, JSObject *obj, jsval idval, jsval *vp)
{
    DirectoryInformation* data = (DirectoryInformation*) JS_GetPrivate(cx, obj);

    if (!data->descriptor) {
        JS_ReportError(cx, "You have to open a directory first.");
        return JS_FALSE;
    }

    *vp = INT_TO_JSVAL(data->position);

    return JS_TRUE;
}

JSBool
Directory_position_set (JSContext *cx, JSObject *obj, jsval idval, jsval *vp)
{
    DirectoryInformation* data = (DirectoryInformation*) JS_GetPrivate(cx, obj);

    if (!data->descriptor) {
        JS_ReportError(cx, "You have to open a directory first.");
        return JS_FALSE;
    }

    JS_BeginRequest(cx);
    JS_EnterLocalRootScope(cx);

    int position; JS_ValueToInt32(cx, *vp, &position);

    if (position < 0 || position >= data->pointers.size()) {
        const char message[] = "The position is out of range.";
        jsval argv[] = {STRING_TO_JSVAL(
            JS_NewString(cx, JS_strdup(cx, message), strlen(message))
        )};

        JS_SetPendingException(cx, OBJECT_TO_JSVAL(JS_CallFunctionWithNew(
            cx, JSVAL_TO_OBJECT(JS_EVAL(cx, "RangeError"), 1, argv))
        ));
        return JS_FALSE;
    }

    JS_LeaveLocalRootScope(cx);
    JS_EndRequest(cx);

    seekdir(data->descriptor, data->pointers[data->position = position]);

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
    JS_EndRequest(cx);

    DirectoryInformation* data = (DirectoryInformation*) JS_GetPrivate(cx, object);

    if (data->descriptor) {
        JS_ReportError(cx, "A directory is already opened.");
        return JS_FALSE;
    }

    data->path = (path[strlen(path)-1] == '/'
        ? path
        : path + std::string("/"));

    if (stat(data->path.c_str(), &data->desc)) {
        JS_ReportError(cx, "The directory couldn't be found.");
        return JS_FALSE;
    }

    if (!S_ISDIR(data->desc.st_mode)) {
        JS_ReportError(cx, "The path doesn't lead to a directory.");
        return JS_FALSE;
    }

    data->pointers.clear();
    data->position = 0;

    data->descriptor = opendir(data->path.c_str());
    JS_SetPrivate(cx, object, data);

    if (!data->descriptor) {
        JS_ReportError(cx, "An error occurred while opening the directory.");
        return JS_FALSE;
    }

    struct dirent* dir;
    struct stat    file;

    long position = 0;
    long i = 0;
    while ((dir = readdir(data->descriptor))) {
        std::string name = dir->d_name;

        if (name != "." && name != "..") {
            stat((data->path+name).c_str(), &file);
            if (S_ISREG(file.st_mode) || S_ISDIR(file.st_mode)) {
                data->pointers.push_back(position);
                std::cout << i << ": " << name << std::endl;
                i++;
            }
        }

        position = telldir(data->descriptor);
    }

    jsval ret;
    JS_CallFunctionName(cx, object, "_init", 0, NULL, &ret);

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
Directory_static_create (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval)
{

    return JS_TRUE;
}

