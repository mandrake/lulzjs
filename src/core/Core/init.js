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

Abstract = { };

Try = {
    these: function () {
        var returnValue;

        for each (let lambda in arguments) {
            try {
                returnValue = lambda();
                break;
            } catch (e) { }
        }

        return returnValue;
    }
}

// Standard classes stuff
require([
    "Object.so", "Object.js",
        "Class.js", "Function.js", "Date.js", "RegExp.js",
        "Enumerable.js", "String.js", "Template.js", "Array.js",
        "Bytes.js", "Hash.js", "Number.js", "Range.js",
        "JSON.js", "XML.js", "random.js"
]);

[Function, Array, String, Number, RegExp, Date, XML].each(function (obj) {
    obj.__defineProperty__(          "__type__", Class.Normal);
    obj.prototype.__defineProperty__("__type__", Class.Normal);
    Object.extend(obj, Class.Methods, Object.Flags.None);
});

//Program.GCExecution = new PeriodicalExecuter(function(){Program.GC()}, 60);

require("init.so");
