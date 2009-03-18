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

require([
    "System/System.so",
        "System/FileSystem/FileSystem.so", "System/FileSystem/FileSystem.js",
            "System/FileSystem/Permission/Permission.js",
            "System/FileSystem/Permission/Mode.js",
            "System/FileSystem/Time/Time.js",

            "System/FileSystem/File/File.so", "System/FileSystem/File/File.js",
            "Directory.so", "Directory.js",
]);

Directory = System.FileSystem.Directory;

