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

var File = System.FileSystem.File;

File.addMethods((function() {
    function writeLine (str) {
        this.write(str+"\n");
    };

    function readLine () {
        if (this.position == System.FileSystem.File.EndOfFile)
            return null;

        var str = "";
        var ch;
        while ((ch = this.read(1)) != '\n' && !this.isEnd()) {
            str += ch;
        }

        return str;
    };

    function readToEnd () {
        var tmp       = this.position;
        var ret       = this.read(this.size-1);
        this.position = tmp;

        return ret;
    };

    function readAll () {
        return this.readToEnd().split('\n');
    };

    function inspect () {
        try {
            return '#<File: path="{0}", permission={1} size={2}, position={3}>'.format([
                this.path, this.permission, this.size, this.position
            ]);
        }
        catch (e) {
            return '#<File: null>';
        }
    };

    return {
        writeLine: writeLine,
        readLine:  readLine,
        readToEnd: readToEnd,
        readAll:   readAll,
        inspect:   inspect,
    }
})());

Object.extendAttributes(File.prototype, {
    name: { get: function() {
        return System.FileSystem.baseName(this.path);
    }},
}, Object.Flags.None);

})();
