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

Object.extend(Object, {
    inspect: function (obj) {
        try {
            if (typeof obj == 'undefined') {
                return 'undefined'
            }
            else if (obj === null) {
                return 'null';
            }
            else if (Object.is(obj, Array)) {
                return obj.toSource();
            }
            
            return (obj.inspect) ? obj.inspect() : obj.toString();
        }
        catch (e) {
            return e;
        }    
    },

    addGetters: function (to, obj) {
        for (let name in obj) {
            if (to.prototype) {
                to.prototype.__defineGetter__(name, obj[name]);
            }
            else {
                to.__defineGetter__(name, obj[name]);
            }
        }
    },

    addSetters: function (obj) {
        for (let name in obj) {
            if (to.prototype) {
                to.prototype.__defineSetter__(name, obj[name]);
            }
            else {
                to.__defineSetter__(name, obj[name]);
            }
        }
    },

    is: function (obj, type) {
        try {
            if (typeof type == 'string') {
                if (type.trim().match(/;|\(.*\)$/)) {
                    throw "LOL HAX";
                }

                type = eval(type);
            }

            return obj instanceof type;
        }
        catch (e) {
            return false;
        }
    },

    toArray: function (obj) {
        return $A(obj);
    }
});

