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

Object.extend(System.FileSystem, (function() {
    function dirName (file) {
        if (file.count("/") >= 2) {
            return /((\/)?.*?)\//.exec(file)[1];
        }
        else if (file.startsWith("/")) {
            return '/';
        }
        else {
            return '.';
        }
    };

    function baseName (file) {
        if (file == "/" || file == "." || file == "..") {
            return file;
        }

        if (file.endsWith("/")) {
            file = file.substr(0, file.length-1);
        }

        return file.match(/([^\/]*)$/)[1];
    };

    return {
        dirName:  dirName,
        baseName: baseName,
    };
})(), Object.Flags.None);

