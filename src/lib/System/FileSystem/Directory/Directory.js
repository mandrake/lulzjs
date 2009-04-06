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

(function() {

var File      = System.FileSystem.File;
var Directory = System.FileSystem.Directory;

Directory.addMethods((function() {
    function _init () {
        for (let i = 0; i < this.length; i++) {
            this.__defineGetter__(i, new Function("return this.fileAt("+i+")"));
        }
    };

    function inspect () {
        try {
            return '#<Directory: path="{0}", permission={1}, length={2}, position={3}>'.format([
                this.path, this.permission, this.length, this.position
            ]);
        }
        catch (e) {
            return '#<Directory: null>';
        }
    };

    return {
        _init:   _init,
        inspect: inspect,
    };
})());

Directory.addAttributes({
    next: { get: function () {
        return (this.position == this.length-1)
            ? null
            : this[this.position+1];
    }},

    current: { get: function () {
        return this[this.position];
    }},

    previous: { get: function () {
        return (this.position == 0)
            ? null
            : this[this.position-1];
    }},

    name: { get: function() {
        return System.FileSystem.baseName(this.path);
    }},
});

})();
