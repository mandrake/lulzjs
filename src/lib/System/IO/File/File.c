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
    jsval jsParent;
    JS_GetProperty(cx, JS_GetGlobalObject(cx), "System", &jsParent);
    JS_GetProperty(cx, JSVAL_TO_OBJECT(jsParent), "IO", &jsParent);
    JSObject* parent = JSVAL_TO_OBJECT(jsParent);

    JSObject* object = JS_InitClass(
        cx, parent, NULL, &File_class,
        File_constructor, 2, NULL, File_methods, NULL, File_static_methods
    );

    if (object) {
        // Default properties
        jsval property;

        property = INT_TO_JSVAL(EOF);
        JS_SetProperty(cx, parent, "EOF", &property);

        return JS_TRUE;
    }

    return JS_FALSE;
}

JSBool
File_constructor (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval)
{
    const char* fileName;
    const char* mode;

    if (argc != 2 || !JS_ConvertArguments(cx, argc, argv, "ss", &fileName, &mode)) {
        JS_ReportError(cx, "File requires the path and the mode as arguments.");
        return JS_FALSE;
    }

    FileInformation* data = JS_malloc(cx, sizeof(FileInformation));
    data->path            = JS_strdup(cx, fileName);
    data->mode            = JS_strdup(cx, mode);
    data->descriptor      = fopen(data->path, data->mode);
    JS_SetPrivate(cx, object, data);

    if (!data->descriptor) {
        JS_ReportError(cx, "An error occurred while opening the file.");
        return JS_FALSE;
    }

    return JS_TRUE;
}

void
File_finalize (JSContext* cx, JSObject* object)
{
    FileInformation* data = JS_GetPrivate(cx, object);

    if (data) {
        JS_free(cx, data->path);
        JS_free(cx, data->mode);

        if (data->descriptor) {
            fclose(data->descriptor);
        }

        JS_free(cx, data);
    }
}

JSBool
File_close (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval)
{
    FileInformation* data = JS_GetPrivate(cx, object);

    if (data->descriptor) {
        fclose(data->descriptor);
        data->descriptor = NULL;
    }

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

    FileInformation* data = JS_GetPrivate(cx, object);

    unsigned offset = 0;
    while (offset < strlen(string)) {
        offset += fwrite((string+offset), sizeof(char), strlen(string)-offset, data->descriptor);
    }

    return JS_TRUE;
}

JSBool
File_read (JSContext *cx, JSObject *object, uintN argc, jsval *argv, jsval *rval)
{
    const unsigned size;

    if (argc != 1 || !JS_ConvertArguments(cx, argc, argv, "u", &size)) {
        JS_ReportError(cx, "Not enough parameters.");
        return JS_FALSE;
    }

    FileInformation* data = JS_GetPrivate(cx, object);

    if (feof(data->descriptor)) {
        *rval = JSVAL_FALSE;
        return JS_TRUE;
    }

    char* string = JS_malloc(cx, (size+1)*sizeof(char));
    memset(string, 0, size+1);
    fread(string, sizeof(char), size, data->descriptor);

    *rval = STRING_TO_JSVAL(JS_NewString(cx, string, size));

    return JS_TRUE;
}

JSBool
File_writeBytes (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval)
{
    JSObject* obj;

    if (argc != 1 || !JS_ConvertArguments(cx, argc, argv, "o", &obj)) {
        JS_ReportError(cx, "Not enough parameters.");
        return JS_FALSE;
    }

    FileInformation* data = JS_GetPrivate(cx, object);

    unsigned char* string;

    jsval ret; JS_CallFunctionName(cx, obj, "toArray", 0, NULL, &ret);
    JSObject* array = JSVAL_TO_OBJECT(ret);

    jsuint length; JS_GetArrayLength(cx, array, &length);
    string = JS_malloc(cx, length*sizeof(char));

    jsuint i;
    for (i = 0; i < length; i++) {
        jsval val; JS_GetElement(cx, array, i, &val);
        string[i] = (unsigned char) JSVAL_TO_INT(val);
    }

    unsigned offset = 0;
    while (offset < length) {
        offset += fwrite((string-offset), sizeof(char), length-offset, data->descriptor);
    }

    return JS_TRUE;
}

JSBool
File_readBytes (JSContext *cx, JSObject *object, uintN argc, jsval *argv, jsval *rval)
{
    const unsigned size;

    JS_BeginRequest(cx);

    if (argc != 1 || !JS_ConvertArguments(cx, argc, argv, "u", &size)) {
        JS_ReportError(cx, "Not enough parameters.");
        return JS_FALSE;
    }

    FileInformation* data = JS_GetPrivate(cx, object);

    if (feof(data->descriptor)) {
        *rval = JSVAL_FALSE;
        return JS_TRUE;
    }

    unsigned char* string = JS_malloc(cx, (size+1)*sizeof(char));
    memset(string, 0, size+1);
    fread(string, sizeof(char), size, data->descriptor);

    JSObject* array = JS_NewArrayObject(cx, 0, NULL);

    unsigned i;
    for (i = 0; i < size; i++) {
        jsval val = INT_TO_JSVAL(string[i]);
        JS_SetElement(cx, array, i, &val);
    }

    jsval newArgv[] = {OBJECT_TO_JSVAL(array)};

    jsval property; JS_GetProperty(cx, JS_GetGlobalObject(cx), "Bytes", &property);
    JSObject* Bytes = JSVAL_TO_OBJECT(property);

    jsval jsProto; JS_GetProperty(cx, Bytes, "prototype", &jsProto);
    JSObject* proto = JSVAL_TO_OBJECT(jsProto);

    JSObject* bytes = JS_ConstructObject(cx, NULL, proto, NULL);
    jsval ret;
    JS_CallFunctionValue(cx, bytes, OBJECT_TO_JSVAL(Bytes), 1, newArgv, &ret);

    *rval = OBJECT_TO_JSVAL(bytes);
    JS_EndRequest(cx);
    return JS_TRUE;
}


JSBool
File_isEnd (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval)
{
    FileInformation* data = JS_GetPrivate(cx, object);

    *rval = BOOLEAN_TO_JSVAL(feof(data->descriptor));
    return JS_TRUE;
}

JSBool
File_static_exists (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval)
{
    const char* path;

    if (argc != 1 || !JS_ConvertArguments(cx, argc, argv, "s", &path)) {
        JS_ReportError(cx, "Not enough parameters.");
        return JS_FALSE;
    }

    FILE* file = fopen(path, "r");
    if (file) {
        *rval = JSVAL_TRUE;
        fclose(file);
    }
    else {
        *rval = JSVAL_FALSE;
    }

    return JS_TRUE;
}

