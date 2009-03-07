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
        return '#<Directory: path="{0}", length={1}, position={2}>'.format([
            this.path, this.length, this.position
        ]);
    };

    return {
        _init:   _init,
        inspect: inspect,
    };
})(), Object.Flags.None);

Directory.prototype.addAttributes({
    next: { get: function () {
        return (this.position == this.length-1)
            ? null
            : this[this.position];
    }},

    current: { get: function () {
        return this[this.position];
    }},

    previous: { get: function () {
        return (this.position == 0)
            ? null
            : this[this.position];
    }},

    name: { get: function() {
        return System.FileSystem.baseName(this.path);
    }},
});

})();
