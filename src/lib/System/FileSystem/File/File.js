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

Object.extend(System.FileSystem.File.prototype, (function() {
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
        return this.read(this.size);
    };

    function readAll () {
        return this.readToEnd().split('\n');
    };

    function inspect () {
        return '#<File: path="{0}", size={1}, position={2}>'.format([
            this.path, this.size, this.position
        ]);
    };

    return {
        writeLine: writeLine,
        readLine:  readLine,
        readToEnd: readToEnd,
        readAll:   readAll,
        inspect:   inspect,
    }
})(), Object.Flags.None);
