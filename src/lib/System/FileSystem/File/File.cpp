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

#include "File.h"

JSBool exec (JSContext* cx) { return File_initialize(cx); }

JSBool
File_initialize (JSContext* cx)
{
    JS_BeginRequest(cx);
    JS_EnterLocalRootScope(cx);

    jsval jsParent   = JS_EVAL(cx, "System.FileSystem");
    JSObject* parent = JSVAL_TO_OBJECT(jsParent);

    JSObject* object = JS_InitClass(
        cx, parent, NULL, &File_class,
        File_constructor, 2, File_attributes, File_methods, NULL, File_static_methods
    );

    if (object) {
        jsval property;

        JSObject* Mode = JS_NewObject(cx, NULL, NULL, NULL);
        property       = OBJECT_TO_JSVAL(Mode);
        JS_SetProperty(cx, object, "Mode", &property);
            property = INT_TO_JSVAL(MODE_READ);
            JS_SetProperty(cx, Mode, "Read", &property);
            property = INT_TO_JSVAL(MODE_WRITE);
            JS_SetProperty(cx, Mode, "Write", &property);
            property = INT_TO_JSVAL(MODE_APPEND);
            JS_SetProperty(cx, Mode, "Append", &property);

        property = INT_TO_JSVAL(EOF);
        JS_SetProperty(cx, object, "EndOfFile", &property);

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
    JS_SetPrivate(cx, object, data);

    if (argc) {
        jsval ret;
        JS_CallFunctionName(cx, object, "open", argc, argv, &ret);
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
        free(data->path);

        if (data->descriptor) {
            fclose(data->descriptor);
        }

        delete data;
    }

    JS_EndRequest(cx);
}

// FIXME: I can't use 64 bit descriptors because javascript is fail
//        on the number side :(
JSBool
File_position_get (JSContext *cx, JSObject *obj, jsval idval, jsval *vp)
{
    FileInformation* data = (FileInformation*) JS_GetPrivate(cx, obj);

    if (!data->descriptor) {
        JS_ReportError(cx, "You have to open a file first.");
        return JS_FALSE;
    }
    
    *vp = INT_TO_JSVAL(ftell(data->descriptor));
    return JS_TRUE;
}

// FIXME: I can't use 64 bit descriptors because javascript is fail
//        on the number side :(
JSBool
File_position_set (JSContext *cx, JSObject *obj, jsval idval, jsval *vp)
{
    FileInformation* data = (FileInformation*) JS_GetPrivate(cx, obj);

    if (!data->descriptor) {
        JS_ReportError(cx, "You have to open a file first.");
        return JS_FALSE;
    }

    JS_BeginRequest(cx);
    int32 offset; JS_ValueToInt32(cx, *vp, &offset);

    if (offset > ftell(data->descriptor)) {
        JS_ReportError(cx, "The offset is bigger than the file's length.");
        return JS_FALSE;
    }
    JS_EndRequest(cx);

    fseek(data->descriptor, offset, SEEK_SET);
    return JS_TRUE;
}

JSBool
File_length_get (JSContext *cx, JSObject *obj, jsval idval, jsval *vp)
{
    FileInformation* data = (FileInformation*) JS_GetPrivate(cx, obj);

    if (!data->descriptor) {
        JS_ReportError(cx, "You have to open a file first.");
        return JS_FALSE;
    }

    *vp = INT_TO_JSVAL(data->desc.st_size);
    return JS_TRUE;
}

JSBool
File_open (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval)
{
    JS_BeginRequest(cx);

    char* filename;
    uint16 mode = MODE_READ;

    if (argc != 1 || !JS_ConvertArguments(cx, argc, argv, "s", &filename)) {
        JS_ReportError(cx, "Not enough parameters.");
        return JS_FALSE;
    }

    if (argc == 2) {
        JS_ValueToUint16(cx, argv[1], &mode);
    }

    FileInformation* data = (FileInformation*) JS_GetPrivate(cx, object);

    if (data->descriptor) {
        JS_ReportError(cx, "A file is already opened.");
        return JS_FALSE;
    }

    data->path = strdup(filename);
    data->mode = mode;

    if (stat(data->path, &data->desc)) {
        JS_ReportError(cx, "An error occurred while opening the file.");
        return JS_FALSE;
    }

    if (!S_ISREG(data->desc.st_mode)) {
        JS_ReportError(cx, "The path doesn't lead to a regular file.");
        return JS_FALSE;
    }

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
    data->descriptor = fopen64(data->path, realMode.c_str());

    JS_EndRequest(cx);
    return JS_TRUE;
}

JSBool
File_close (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval)
{
    JS_BeginRequest(cx);

    FileInformation* data = (FileInformation*) JS_GetPrivate(cx, object);

    if (data->descriptor) {
        fclose(data->descriptor);
        data->descriptor = NULL;
        free(data->path);
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
    char* string;

    if (argc != 1 || !JS_ConvertArguments(cx, argc, argv, "s", &string)) {
        JS_ReportError(cx, "Not enough parameters.");
        return JS_FALSE;
    }

    FileInformation* data = (FileInformation*) JS_GetPrivate(cx, object);

    if (!data->descriptor) {
        JS_ReportError(cx, "You have to open a file first.");
        return JS_FALSE;
    }

    unsigned offset = 0;
    while (offset < strlen(string)) {
        offset += fwrite((string+offset), sizeof(char), strlen(string)-offset, data->descriptor);
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

    if (feof(data->descriptor)) {
        *rval = JSVAL_FALSE;
        return JS_TRUE;
    }

    JS_EnterLocalRootScope(cx);
    char* string = (char*) JS_malloc(cx, (size+1)*sizeof(char));
    memset(string, 0, size+1);
    fread(string, sizeof(char), size, data->descriptor);
    *rval = STRING_TO_JSVAL(JS_NewString(cx, string, size));
    JS_LeaveLocalRootScope(cx);

    JS_EndRequest(cx);
    return JS_TRUE;
}

JSBool
File_writeBytes (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval)
{
    JS_BeginRequest(cx);

    JSObject* obj;

    if (argc != 1 || !JS_ConvertArguments(cx, argc, argv, "o", &obj)) {
        JS_ReportError(cx, "Not enough parameters.");

        JS_EndRequest(cx);
        return JS_FALSE;
    }

    FileInformation* data = (FileInformation*) JS_GetPrivate(cx, object);

    unsigned char* string;

    JS_EnterLocalRootScope(cx);
    jsval ret; JS_CallFunctionName(cx, obj, "toArray", 0, NULL, &ret);
    JSObject* array = JSVAL_TO_OBJECT(ret);

    jsuint length; JS_GetArrayLength(cx, array, &length);
    string = new unsigned char[length];

    jsuint i;
    for (i = 0; i < length; i++) {
        jsval val; JS_GetElement(cx, array, i, &val);
        string[i] = (unsigned char) JSVAL_TO_INT(val);
    }

    unsigned offset = 0;
    while (offset < length) {
        offset += fwrite((string-offset), sizeof(char), length-offset, data->descriptor);
    }
    delete string;

    JS_LeaveLocalRootScope(cx);
    JS_EndRequest(cx);
    return JS_TRUE;
}

JSBool
File_readBytes (JSContext *cx, JSObject *object, uintN argc, jsval *argv, jsval *rval)
{
    JS_BeginRequest(cx);

    unsigned size;

    if (argc != 1 || !JS_ConvertArguments(cx, argc, argv, "u", &size)) {
        JS_ReportError(cx, "Not enough parameters.");

        JS_EndRequest(cx);
        return JS_FALSE;
    }

    FileInformation* data = (FileInformation*) JS_GetPrivate(cx, object);

    if (feof(data->descriptor)) {
        *rval = JSVAL_FALSE;

        JS_EndRequest(cx);
        return JS_TRUE;
    }

    JS_EnterLocalRootScope(cx);
    unsigned char* string = new unsigned char[size+1];
    memset(string, 0, size+1);
    fread(string, sizeof(char), size, data->descriptor);

    JSObject* array = JS_NewArrayObject(cx, 0, NULL);

    unsigned i;
    for (i = 0; i < size; i++) {
        jsval val = INT_TO_JSVAL(string[i]);
        JS_SetElement(cx, array, i, &val);
    }
    delete string;

    jsval newArgv[] = {OBJECT_TO_JSVAL(array)};

    jsval property; JS_GetProperty(cx, JS_GetGlobalObject(cx), "Bytes", &property);
    JSObject* Bytes = JSVAL_TO_OBJECT(property);

    jsval jsProto; JS_GetProperty(cx, Bytes, "prototype", &jsProto);
    JSObject* proto = JSVAL_TO_OBJECT(jsProto);

    JSObject* bytes = JS_ConstructObject(cx, NULL, proto, NULL);
    jsval ret;
    JS_CallFunctionValue(cx, bytes, OBJECT_TO_JSVAL(Bytes), 1, newArgv, &ret);

    *rval = OBJECT_TO_JSVAL(bytes);

    JS_LeaveLocalRootScope(cx);
    JS_EndRequest(cx);
    return JS_TRUE;
}

