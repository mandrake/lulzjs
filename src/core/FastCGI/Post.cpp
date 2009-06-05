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

#include "Post.h"

#define POST_NORMAL     "application/x-www-form-urlencoded"
#define POST_XML        "text/xml"
#define POST_MULTIPART  "multipart/form-data"

JSBool
Post_initialize (JSContext* cx)
{
    if (strcasecmp(getenv("REQUEST_METHOD"), "POST") != 0) {
        return JS_TRUE;
    }

    JSBool result = JS_FALSE;

    JS_BeginRequest(cx);
    JS_EnterLocalRootScope(cx);

    JS_LeaveLocalRootScope(cx);
    JS_EndRequest(cx);

    return result;
}

