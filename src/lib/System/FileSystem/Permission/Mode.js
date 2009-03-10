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

var FileSystem = System.FileSystem;
var Permission = FileSystem.Permission

Permission.Mode = Class.create({
    constructor: function (mode) {
        this.__defineProperty__("mode", String(mode).toNumber(), Object.Flags.None);
    },

    methods: {
        toString: function () {
            var str = new Array;

            str.push(this.mode & System.FileSystem.Permission.Mode.Read    ? "r" : "-");
            str.push(this.mode & System.FileSystem.Permission.Mode.Write   ? "w" : "-");
            str.push(this.mode & System.FileSystem.Permission.Mode.Execute ? "x" : "-");

            return str.join('');
        },

        inspect: function () {
            return '#<Mode: mode={0}, string="{1}">'.format([this.mode, this.toString()]);
        },
    },

    static: {
        Execute: 1,
        Write:   2,
        Read:    4,
    }
});

})();
