/****************************************************************************
* This file is part of lulzJS                                               *
* Copyleft meh. [http://meh.doesntexist.org | meh.ffff@gmail.com]           *
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

jsval File;
jsval Directory;
jsval Permission;
jsval Time;

JSBool exec (JSContext* cx) { return Directory_initialize(cx); }

JSBool
Directory_initialize (JSContext* cx)
{
    JS_BeginRequest(cx);
    JS_EnterLocalRootScope(cx);

    jsval jsParent   = JS_EVAL(cx, "System.FileSystem");
    JSObject* parent = JSVAL_TO_OBJECT(jsParent);

    JSObject* object = JS_InitClass(
        cx, parent, JSVAL_TO_OBJECT(JS_EVAL(cx, "Object.extend({}, Enumerable, Object.Flags.None)")), &Directory_class,
        Directory_constructor, 1, Directory_attributes, Directory_methods, NULL, Directory_static_methods
    );

    if (object) {
        File       = JS_EVAL(cx, "System.FileSystem.File");
        Directory  = JS_EVAL(cx, "System.FileSystem.Directory");
        Permission = JS_EVAL(cx, "System.FileSystem.Permission"); 
        Time       = JS_EVAL(cx, "System.FileSystem.Time");

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
    JS_SetPrivate(cx, object, data);

    data->descriptor = NULL;
    data->position   = 0;

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
Directory_opened_get (JSContext *cx, JSObject *obj, jsval idval, jsval *vp)
{
    DirectoryInformation* data = (DirectoryInformation*) JS_GetPrivate(cx, obj);

    if (data->descriptor) {
        *vp = JSVAL_TRUE;
    }
    else {
        *vp = JSVAL_FALSE;
    }

    return JS_TRUE;
}

JSBool
Directory_name_get (JSContext *cx, JSObject *obj, jsval idval, jsval *vp)
{
    DirectoryInformation* data = (DirectoryInformation*) JS_GetPrivate(cx, obj);

    if (data->path.empty()) {
        JS_ReportError(cx, "There is no directory initialized.");
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
Directory_path_get (JSContext *cx, JSObject *obj, jsval idval, jsval *vp)
{
    DirectoryInformation* data = (DirectoryInformation*) JS_GetPrivate(cx, obj);

    if (data->path.empty()) {
        JS_ReportError(cx, "There is no directory initialized.");
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

    if (!data->descriptor) {
        *vp = JSVAL_NULL;
    }
    else {
        *vp = INT_TO_JSVAL(data->pointers.size());
    }

    return JS_TRUE;
}

JSBool
Directory_position_get (JSContext *cx, JSObject *obj, jsval idval, jsval *vp)
{
    DirectoryInformation* data = (DirectoryInformation*) JS_GetPrivate(cx, obj);

    if (!data->descriptor) {
        *vp = JSVAL_NULL;
    }
    else {
        *vp = INT_TO_JSVAL(data->position);
    }

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

        JSObject* exception; JS_CallFunctionWithNew(
            cx, JS_EVAL(cx, "RangeError"), 1, argv, &exception
        );
        JS_SetPendingException(cx, OBJECT_TO_JSVAL(exception));
        return JS_FALSE;
    }

    JS_LeaveLocalRootScope(cx);
    JS_EndRequest(cx);

    seekdir(data->descriptor, data->pointers[(data->position = position)]);

    return JS_TRUE;
}

JSBool
Directory_size_get (JSContext *cx, JSObject *obj, jsval idval, jsval *vp)
{
    DirectoryInformation* data = (DirectoryInformation*) JS_GetPrivate(cx, obj);
    
    JS_NewNumberValue(cx, data->desc.st_size, vp);
    return JS_TRUE;
}

JSBool
Directory_permission_get (JSContext *cx, JSObject *obj, jsval idval, jsval *vp)
{
    DirectoryInformation* data = (DirectoryInformation*) JS_GetPrivate(cx, obj);

    JS_BeginRequest(cx);
    JS_EnterLocalRootScope(cx);

    char bitString[10]; snprintf(bitString, 9, "%lo", (long unsigned) data->desc.st_mode);
    int  bits = atoi(bitString);

    // % 10000 gets the file mode bits.
    jsval argv[] = {INT_TO_JSVAL(bits % 10000)};
    JSObject* permission;

    if (!JS_CallFunctionWithNew(cx, Permission, 1, argv, &permission)) {
        if (!JS_IsExceptionPending(cx)) {
            JS_ReportError(cx, "Something went wrong with the creation of the object.");
        }

        return JS_FALSE;
    }

    *vp = OBJECT_TO_JSVAL(permission);

    JS_LeaveLocalRootScope(cx);
    JS_EndRequest(cx);
    return JS_TRUE;
}

JSBool
Directory_last_get (JSContext *cx, JSObject *obj, jsval idval, jsval *vp)
{
    DirectoryInformation* data = (DirectoryInformation*) JS_GetPrivate(cx, obj);

    JS_BeginRequest(cx);
    JS_EnterLocalRootScope(cx);

    jsval access, modification, change;
    JS_NewNumberValue(cx, data->desc.st_atime*1000, &access);
    JS_NewNumberValue(cx, data->desc.st_mtime*1000, &modification);
    JS_NewNumberValue(cx, data->desc.st_ctime*1000, &change);

    jsval argv[] = {access, modification, change};
    JSObject* last;

    if (!JS_CallFunctionWithNew(cx, Time, 3, argv, &last)) {
        if (!JS_IsExceptionPending(cx)) {
            JS_ReportError(cx, "Something went wrong with the creation of the object.");
        }

        return JS_FALSE;
    }

    *vp = OBJECT_TO_JSVAL(last);

    JS_LeaveLocalRootScope(cx);
    JS_EndRequest(cx);
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
        : std::string(path) + "/");

    if (stat(data->path.c_str(), &data->desc)) {
        JS_ReportError(cx, "The directory couldn't be found.");
        return JS_FALSE;
    }

    if (!S_ISDIR(data->desc.st_mode)) {
        JS_ReportError(cx, "The path doesn't lead to a directory.");
        return JS_FALSE;
    }

    data->descriptor = opendir(data->path.c_str());

    if (!data->descriptor) {
        return JS_TRUE;
    }

    struct dirent* dir;
    struct stat    file;

    long position = 0;
    while ((dir = readdir(data->descriptor))) {
        std::string name = dir->d_name;

        if (name != "." && name != "..") {
            stat((data->path+name).c_str(), &file);
            if (S_ISREG(file.st_mode) || S_ISDIR(file.st_mode)) {
                data->pointers.push_back(position);
            }
        }

        position = telldir(data->descriptor);
    }

    jsval ret;
    JS_CallFunctionName(cx, object, "_init", 0, NULL, &ret);

    *rval = OBJECT_TO_JSVAL(object);

    return JS_TRUE;
}

JSBool
Directory_close (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval)
{
    JS_BeginRequest(cx);

    DirectoryInformation* data = (DirectoryInformation*) JS_GetPrivate(cx, object);

    if (data) {
        if (data->descriptor) {
            closedir(data->descriptor);
            data->descriptor = NULL;
            data->path       = "";
            data->position   = 0;
            data->pointers.clear();
            memset(&data->desc, 0, sizeof(struct stat));
        }
    }

    JS_EndRequest(cx);
    return JS_TRUE;
}

JSBool
Directory_fileAt (JSContext* cx, JSObject* obj, uintN argc, jsval* argv, jsval* rval)
{
    JS_BeginRequest(cx);

    int position;

    if (argc != 1 || !JS_ConvertArguments(cx, argc, argv, "i", &position)) {
        JS_ReportError(cx, "Not enough parameters.");

        JS_EndRequest(cx);
        return JS_FALSE;
    }
    jsrefcount req = JS_SuspendRequest(cx);

    DirectoryInformation* data = (DirectoryInformation*) JS_GetPrivate(cx, obj);

    if (position < 0 || position > data->pointers.size()) {
        JS_ResumeRequest(cx, req);
        JS_EnterLocalRootScope(cx);

        const char message[] = "The position is out of range.";
        jsval argv[] = {STRING_TO_JSVAL(
            JS_NewString(cx, JS_strdup(cx, message), strlen(message))
        )};

        JSObject* exception; JS_CallFunctionWithNew(
            cx, JS_EVAL(cx, "RangeError"), 1, argv, &exception
        );
        JS_SetPendingException(cx, OBJECT_TO_JSVAL(exception));

        JS_LeaveLocalRootScope(cx);
        JS_EndRequest(cx);
        return JS_FALSE;
    }
 
    if (!data->descriptor) {
        JS_ReportError(cx, "There is no opened directory.");

        JS_ResumeRequest(cx, req);
        JS_EndRequest(cx);
        return JS_FALSE;
    }
 
    struct dirent* dir;
    struct stat file;
    std::string path;
 
    seekdir(data->descriptor, data->pointers[position]);
    dir  = readdir(data->descriptor);
    path = data->path + dir->d_name;
    stat(path.c_str(), &file);

    JS_ResumeRequest(cx, req);
    JS_EnterLocalRootScope(cx);

    std::string newPath = data->path + dir->d_name;
    jsval newArgv[] = {
        STRING_TO_JSVAL(JS_NewString(cx, JS_strdup(cx, newPath.c_str()), newPath.length()))
    };
    
    JSObject* retObject = NULL;
    jsval     classObj;
    if (S_ISREG(file.st_mode)) {
        classObj = File;
    }
    else if (S_ISDIR(file.st_mode)) {
        classObj = Directory;
    }

    if (JS_IsExceptionPending(cx)) {
        return JS_FALSE;
    }

    if (!JS_CallFunctionWithNew(cx, classObj, 1, newArgv, &retObject)) {
        if (!JS_IsExceptionPending(cx)) {
            JS_ReportError(cx, "Something went wrong with the creation of the object.");
        }

        return JS_FALSE;
    }

    *rval = OBJECT_TO_JSVAL(retObject);
 
    JS_LeaveLocalRootScope(cx);
    JS_EndRequest(cx);
    return JS_TRUE;
}

JSBool
Directory_static_create (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval)
{

    return JS_TRUE;
}

