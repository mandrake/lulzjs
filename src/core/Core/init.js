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
require(["Object.so", "Object.js"]);

require([
    "Class.js", "Function.js", "Date.js", "RegExp.js",
    "String.js", "Template.js", "Enumerable.js", "Array.js",
    "Bytes.js", "Hash.js", "Number.js",
    "Range.js", "XML.js", "Thread", "random.js"
]);

[Function, Array, String, Number, RegExp, Date, XML].each(function (obj) {
    obj.__defineProperty__(          "__type__", Class.Normal);
    obj.prototype.__defineProperty__("__type__", Class.Normal);
    Object.extend(obj, Class.Methods, Object.Flags.None);
});

//Program.GCExecution = new PeriodicalExecuter(function(){Program.GC()}, 60);

// Isolate the used variables.
(function(){
    require("System/System.so");
    require("System/Environment/Environment.so");

    // Include the environment paths in the current script.
    var PATH = System.Environment.JSPATH;
    if (PATH) {
        let re = /([^:])+/g;
    
        let path;
        while (path = re.exec(PATH)) {
            __PATH__.push(path[0]);
        }
    }

    // Include the standards include following the environment variable.
    var INCLUDE = System.Environment.JSINCLUDE;
    if (INCLUDE) {
        let re = /([^:])+/g;
    
        let file;
        while (file = re.exec(INCLUDE)) {
            include(file[0]);
        }
    }
})();
