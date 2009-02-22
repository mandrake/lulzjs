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

    function readNext (type) {
        if (type == File) {
            this.position = this.positions.files+1;

            while (this.next) {
                if (Object.is(this.current, File)) {
                    this.positions.files = this.position;
                    return this.current;
                }
                this.position++;
            }
        }
        else if (type == Directory) {
            this.position = this.positions.directories+1;

            while (this.next) {
                if (Object.is(this.current, Directory)) {
                    this.positions.directories = this.position;
                    return this.current;
                }
                this.position++;
            }
        }
        else {
            this.position = this.positions.all+1;

            while (this.next) {
                this.positions.all = this.position;
                return this.current;
            }
        }

        return null;
    }

    return {
        readNext: readNext
    };
})(), Object.Flags.None);
