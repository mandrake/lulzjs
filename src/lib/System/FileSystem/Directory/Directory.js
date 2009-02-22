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

Object.extend(System.FileSystem.Directory.prototype, {
    positions: {
        all:         0,
        files:       0,
        directories: 0
    }
}, Object.Flags.None);

Object.extend(System.FileSystem.Directory.prototype, (function() {
    var File      = System.FileSystem.File;
    var Directory = System.FileSystem.Directory;

    function _init () {
        for (let i = 0; i < this.length; i++) {
            this.__defineGetter__(i, new Function("return this.fileAt("+i+")"));
        }
    };
    
    function fileAt (index) {
        if (index < 0 || index >= this.length) {
            throw new Error("The index is too large or negative.");
        }

        var tmpPosition = this.position;
        this.position   = index;
        var tmp         = this.current;
        this.position   = tmpPosition;

        return tmp;

    };

    return {
        _init:        _init,
        fileAt:       fileAt,
    };
})(), Object.Flags.None);
