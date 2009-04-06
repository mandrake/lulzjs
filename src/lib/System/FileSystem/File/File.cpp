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

#include "File.h"

jsval Permission;
jsval Time;

JSBool exec (JSContext* cx) { return File_initialize(cx); }

JSBool
File_initialize (JSContext* cx)
{
    JS_BeginRequest(cx);
    JS_EnterLocalRootScope(cx);

    JSObject* parent = JSVAL_TO_OBJECT(JS_EVAL(cx, "System.FileSystem"));

    JSObject* object = JS_InitClass(
        cx, parent, NULL, &File_class,
        File_constructor, 1, File_attributes, File_methods, NULL, File_static_methods
    );

    if (object) {
        jsval property;

        Permission = JS_EVAL(cx, "System.FileSystem.Permission"); 
        Time       = JS_EVAL(cx, "System.FileSystem.Time");

        JSObject* File = JSVAL_TO_OBJECT(JS_EVAL(cx, "System.FileSystem.File"));

        JSObject* Mode = JS_NewObject(cx, NULL, NULL, NULL);
        JS_DefineProperty(cx, File, "Mode", OBJECT_TO_JSVAL(Mode), NULL, NULL, JSPROP_READONLY);
            property = INT_TO_JSVAL(MODE_NONE);
            JS_SetProperty(cx, Mode, "None", &property);
            property = INT_TO_JSVAL(MODE_READ);
            JS_SetProperty(cx, Mode, "Read", &property);
            property = INT_TO_JSVAL(MODE_WRITE);
            JS_SetProperty(cx, Mode, "Write", &property);
            property = INT_TO_JSVAL(MODE_APPEND);
            JS_SetProperty(cx, Mode, "Append", &property);

        JS_DefineProperty(cx, File, "End", INT_TO_JSVAL(EOF), NULL, NULL, JSPROP_READONLY);

        JS_LeaveLocalRootScope(cx);
        JS_EndRequest(cx);
        return JS_TRUE;
    }

    return JS_FALSE;
}

JSBool
File_constructor (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval)
{
    JS_BeginRequest(cx);

    FileInformation* data = new FileInformation;
    data->descriptor      = NULL;
    data->desc            = NULL;
    JS_SetPrivate(cx, object, data);

    if (argc) {
        jsval ret;
        JS_CallFunctionName(cx, object, "open", argc, argv, &ret);

        if (JS_IsExceptionPending(cx)) {
            JS_ReportPendingException(cx);
            return JS_FALSE;
        }
    }

    JS_EndRequest(cx);
    return JS_TRUE;
}

void
File_finalize (JSContext* cx, JSObject* object)
{
    JS_BeginRequest(cx);

    FileInformation* data = (FileInformation*) JS_GetPrivate(cx, object);

    if (data) {
        if (data->descriptor) {
            fclose(data->descriptor);
        }
        if (data->desc) {
            delete data->desc;
        }

        delete data;
    }

    JS_EndRequest(cx);
}

JSBool
File_opened_get (JSContext *cx, JSObject *obj, jsval idval, jsval *vp)
{
    FileInformation* data = (FileInformation*) JS_GetPrivate(cx, obj);

    if (data->descriptor) {
        *vp = JSVAL_TRUE;
    }
    else {
        *vp = JSVAL_FALSE;
    }

    return JS_TRUE;
}

JSBool
File_path_get (JSContext *cx, JSObject *obj, jsval idval, jsval *vp)
{
    FileInformation* data = (FileInformation*) JS_GetPrivate(cx, obj);

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
File_path_set (JSContext *cx, JSObject *obj, jsval idval, jsval *vp)
{
    JSBool check = JS_TRUE;

    JS_BeginRequest(cx);
    JS_EnterLocalRootScope(cx);

    jsval argv[] = {*vp};
    jsval ret;

    JS_CallFunctionName(cx, obj, "close", 0, NULL, &ret);
    JS_CallFunctionName(cx, obj, "open", 1, argv, &ret);

    if (JS_IsExceptionPending(cx)) {
        check = JS_FALSE;
    }

    JS_LeaveLocalRootScope(cx);
    JS_EndRequest(cx);
    return check;
}

JSBool
File_position_get (JSContext *cx, JSObject *obj, jsval idval, jsval *vp)
{
    FileInformation* data = (FileInformation*) JS_GetPrivate(cx, obj);

    if (!data->descriptor) {
        *vp = JSVAL_NULL;
    }
    else {
        data->position = ftello(data->descriptor);
        JS_NewNumberValue(cx, data->position, vp);
    }

    return JS_TRUE;
}

JSBool
File_position_set (JSContext *cx, JSObject *obj, jsval idval, jsval *vp)
{
    FileInformation* data = (FileInformation*) JS_GetPrivate(cx, obj);

    if (!data->descriptor) {
        JS_ReportError(cx, "You have to open a file first.");
        return JS_FALSE;
    }

    JS_BeginRequest(cx);
    jsdouble offset; JS_ValueToNumber(cx, *vp, &offset);

    if (offset > data->desc->st_size) {
        JS_ReportError(cx, "The offset is bigger than the file size.");
        return JS_FALSE;
    }
    JS_EndRequest(cx);

    fseeko(data->descriptor, (data->position = offset), SEEK_SET);
    return JS_TRUE;
}

JSBool
File_size_get (JSContext *cx, JSObject *obj, jsval idval, jsval *vp)
{
    FileInformation* data = (FileInformation*) JS_GetPrivate(cx, obj);

    if (!data->desc) {
        *vp = JSVAL_NULL;
        return JS_TRUE;
    }
    
    JS_NewNumberValue(cx, data->desc->st_size, vp);
    return JS_TRUE;
}

JSBool
File_permission_get (JSContext *cx, JSObject *obj, jsval idval, jsval *vp)
{
    FileInformation* data = (FileInformation*) JS_GetPrivate(cx, obj);

    if (!data->desc) {
        *vp = JSVAL_NULL;
        return JS_TRUE;
    }

    JS_BeginRequest(cx);
    JS_EnterLocalRootScope(cx);

    char bitString[10]; snprintf(bitString, 9, "%lo", (long unsigned) data->desc->st_mode);
    int  bits = atoi(bitString);

    // % 1000 gets the file mode bits.
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
File_last_get (JSContext *cx, JSObject *obj, jsval idval, jsval *vp)
{
    FileInformation* data = (FileInformation*) JS_GetPrivate(cx, obj);

    JS_BeginRequest(cx);
    JS_EnterLocalRootScope(cx);

    jsval access, modification, change;
    JS_NewNumberValue(cx, data->desc->st_atime*1000, &access);
    JS_NewNumberValue(cx, data->desc->st_mtime*1000, &modification);
    JS_NewNumberValue(cx, data->desc->st_ctime*1000, &change);

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
File_open (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval)
{
    JS_BeginRequest(cx);
    JS_EnterLocalRootScope(cx);

    const char* filename;
    int32       mode = MODE_NONE;

    if (argc < 1) {
        JS_ReportError(cx, "Not enough parameters.");
        return JS_FALSE;
    }

    FileInformation* data = (FileInformation*) JS_GetPrivate(cx, object);

    if (argc == 2) {
        JS_ValueToInt32(cx, argv[1], &mode);
        filename = JS_GetStringBytes(JS_ValueToString(cx, argv[0]));
    }
    else {
        if (JSVAL_IS_NUMBER(argv[0])) {
            if (data->path.empty()) {
                JS_ReportError(cx, "The path is missing.");
                return JS_FALSE;
            }

            JS_ValueToInt32(cx, argv[0], &mode);
            filename = data->path.c_str();
        }
        else {
            filename = JS_GetStringBytes(JS_ValueToString(cx, argv[0]));
        }
    }

    if (data->descriptor) {
        JS_ReportError(cx, "A file is already opened.");
        return JS_FALSE;
    }

    data->path = filename;
    data->mode = mode;
    data->desc = new struct stat;

    if (mode == MODE_READ || mode == MODE_NONE) {
        if (stat(data->path.c_str(), data->desc)) {
            JS_ReportError(cx, "The file couldn't be found.");
            return JS_FALSE;
        }

        if (!S_ISREG(data->desc->st_mode)) {
            JS_ReportError(cx, "The path doesn't lead to a regular file.");
            return JS_FALSE;
        }
    }

    if (mode != MODE_NONE) {
        std::string realMode;
        switch (mode) {
            case MODE_READ: 
            realMode = "rb+";
            break;

            case MODE_WRITE:
            realMode = "wb+";
            break;
        
            case MODE_APPEND:
            realMode = "ab+";
            break;
        }
        data->descriptor = fopen64(data->path.c_str(), realMode.c_str());

        if (mode != MODE_READ) {
            stat(data->path.c_str(), data->desc);
        }
    }

    JS_LeaveLocalRootScope(cx);
    JS_EndRequest(cx);

    *rval = OBJECT_TO_JSVAL(object);

    return JS_TRUE;
}

JSBool
File_close (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval)
{
    JS_BeginRequest(cx);

    FileInformation* data = (FileInformation*) JS_GetPrivate(cx, object);

    if (data->desc) {
        delete data->desc;
        data->desc = NULL;
    }

    if (data->descriptor) {
        fclose(data->descriptor);
        data->descriptor = NULL;
        data->path       = "";
    }
    else {
        JS_ReportError(cx, "You have to open a file first.");
        return JS_FALSE;
    }

    JS_EndRequest(cx);
    return JS_TRUE;
}

JSBool
File_write (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval)
{
    JSString* stringObject;

    JS_BeginRequest(cx);
    if (argc < 1 || !JS_ConvertArguments(cx, argc, argv, "S", &stringObject)) {
        JS_ReportError(cx, "Not enough parameters.");
        return JS_FALSE;
    }
    JS_EndRequest(cx);

    FileInformation* data = (FileInformation*) JS_GetPrivate(cx, object);

    if (!data->descriptor) {
        JS_ReportError(cx, "You have to open a file first.");
        return JS_FALSE;
    }

    char*  string = JS_GetStringBytes(stringObject);
    size_t offset = 0;
    size_t length = JS_GetStringLength(stringObject);

    while (offset < length) {
        offset += fwrite((string+offset), sizeof(char), length-offset, data->descriptor);
    }

    return JS_TRUE;
}

JSBool
File_read (JSContext *cx, JSObject *object, uintN argc, jsval *argv, jsval *rval)
{
    JS_BeginRequest(cx);

    unsigned size;

    if (argc != 1 || !JS_ConvertArguments(cx, argc, argv, "u", &size)) {
        JS_ReportError(cx, "Not enough parameters.");
        return JS_FALSE;
    }

    FileInformation* data = (FileInformation*) JS_GetPrivate(cx, object);

    if (!data->descriptor) {
        JS_ReportError(cx, "You have to open a file first.");
        return JS_FALSE;
    }

    if (data->position == EOF) {
        *rval = JSVAL_NULL;

        JS_EndRequest(cx);
        return JS_TRUE;
    }

    JS_EnterLocalRootScope(cx);
    char* string = (char*) JS_malloc(cx, (size+1)*sizeof(char));

    size_t offset = 0;
    while (offset < size) {
        if (feof(data->descriptor)) {
            data->position = EOF;
            break;
        }   

        offset += fread((string+offset), sizeof(char), size-offset, data->descriptor);
    }
    string = (char*) JS_realloc(cx, string, offset*sizeof(char));
    *rval = STRING_TO_JSVAL(JS_NewString(cx, string, offset));

    JS_LeaveLocalRootScope(cx);
    JS_EndRequest(cx);
    return JS_TRUE;
}

