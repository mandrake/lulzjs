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

FileSystem.Time = Class.create({
    constructor: function (access, modification, change) {
        this.access       = new Date(access);
        this.modification = new Date(modification);
        this.change       = new Date(change);
    },

    methods: {
        toString: function () {
            return '#<Time: access="{0}", modification="{1}", change="{2}">'.format([
                this.access, this.modification, this.change
            ]);
        },
    },
});

})();
