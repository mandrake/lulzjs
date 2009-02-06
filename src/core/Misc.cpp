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

#include "Misc.h"

char*
JS_strdup (JSContext* cx, const char* string)
{
    char* new = JS_malloc(cx, strlen(string)*sizeof(char)+1);

    size_t i;
    for (i = 0; i < strlen(string); i++) {
        new[i] = string[i];
    }
    new[i] = '\0';

    return new;
}

const char*
readFile (JSContext* cx, const char* file)
{
    FILE*  fp;
    struct stat fileStat;
    char* text;
    
    if (!stat(file, &fileStat)) {
        fp   = fopen(file, "rb");
        text = JS_malloc(cx, (fileStat.st_size+1)*sizeof(char));

        uint32 offset = 0;
        while (offset < fileStat.st_size) {
            offset += fread((text+offset), sizeof(char), (fileStat.st_size-offset), fp);
        }
        text[fileStat.st_size] = '\0';
    }

    return text;
}

short
fileExists (const char* file)
{
    FILE* check = fopen(file, "r");

    if (check) {
        fclose(check);
        return 1;
    }
    else {
        return 0;
    }
}

const char*
stripRemainder (JSContext* cx, char* text)
{
    char* stripped = NULL;
    short strip = 0;

    size_t position;
    if (text[0] == '#') {
        for (position = 0; text[position] != '\n'; position++) {
            if (text[position] == '!') {
                strip = 1;
            }
        }
    }

    if (strip) {
        stripped = JS_malloc(cx, (strlen(&text[position])+1)*sizeof(char));
        strcpy(stripped, &text[position]);
        JS_free(cx, text);
    }
    else {
        stripped = text;
    }

    return stripped;
}

