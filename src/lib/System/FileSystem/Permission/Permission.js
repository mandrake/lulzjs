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

FileSystem.Permission = Class.create({
    constructor: function (perm) {
        var perms = System.FileSystem.Permission.parse(perm);

        this.__defineProperty__("permission", perms.permission, Object.Flags.None);
        delete perms.permission;

        Object.extend(this, perms);
    },

    methods: {
        toString: function () {
            return this.permission;
        },

        inspect: function () {
            return "#<Permission: bits={0}, user={1}, group={2}, other={3}>".format([
                this.bits, this.user, this.group, this.other
            ]);
        },
    },

    static: {
        parse: function (perm) {
            var umask = System.FileSystem.umask.toArray();
            for (let i in umask) {
                if (umask[i] > 6) {
                    umask[i] = 6;
                }
            }
            umask = umask.join('').fromBase(8);

            var def = (0666 - umask).toPaddedString(4,8);
            def = {
                permission: def,
                bits:       def[0],
                user:       new System.FileSystem.Permission.Mode(def[1]),
                group:      new System.FileSystem.Permission.Mode(def[2]),
                other:      new System.FileSystem.Permission.Mode(def[3]),
            };

            if (!perm.is(Number) && !perm.is(String)) {
                return def;
            }

            perm = perm.toString();
            if (perm.length < 3) {
                return def;
            }

            var offset = (perm.length < 4 ? 0 : 1);

            return {
                permission: perm, 
                bits:       (perm.length < 4 ? '0' : perm[0]).toNumber(),
                user:       new System.FileSystem.Permission.Mode(perm[0+offset]),
                group:      new System.FileSystem.Permission.Mode(perm[1+offset]),
                other:      new System.FileSystem.Permission.Mode(perm[2+offset]),
            };
        },
    }
});

})();
