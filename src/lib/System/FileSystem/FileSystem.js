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
    function dirName (path) {
        if (path.count("/") >= 2 && !(path.count("/") == 2 && path.endsWith("/"))) {
            if (path.endsWith("/")) {
                path = path.substr(0, path.length-1);
            }

            return /((\/)?.*?)\/[^\/]*$/.exec(path)[1];
        }
        else if (path.startsWith("/")) {
            return '/';
        }
        else {
            return '.';
        }
    };

    function baseName (path) {
        if (path == "/" || path == "." || path == "..") {
            return path;
        }

        if (path.endsWith("/")) {
            path = path.substr(0, path.length-1);
        }

        return path.match(/([^\/]*)$/)[1];
    };

    function baseDir (path) {
        return System.FileSystem.baseName(System.FileSystem.dirName(path));
    };

    return {
        dirName:  dirName,
        baseName: baseName,
        baseDir:  baseDir,
    };
})(), Object.Flags.None);

