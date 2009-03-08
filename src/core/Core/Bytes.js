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

Bytes = Class.create(Enumerable, {
    constructor: function (base) {
        if (base !== undefined && !base.is(Array) && !base.is(String)) {
            throw "You have to pass an Array or a String.";
        }

        if (!base) {
            this.array = new Array;
        }
        else {
            if (base.is(Array)) {
                this.array = base;
            }
            else {
                this.array = new Array;
                for each (let ch in base) {
                    this.array.push(ch.toCode());
                }
            }
        }
    },

    methods: {
        append: function (bytes) {
            return this.array.concat(bytes);
        },

        byteAt: function (index) {
            if (index >= this.array.length) {
                throw "Index out of range.";
            }
    
            return this.array[index];
        },
    
        toArray: function () {
            return this.array;
        },
    
        toString: function () {
            var str = new String;
    
            for each (let byte in this.array) {
                str += byte.toChar();
            }
    
            return str;
        },
        
        inspect: function () {
            var str = new String;
    
            for (var i = 0; i < this.array.length; i++) {
                str += "\\x"+this.array[i].toPaddedString(2, 16);
            }
    
            return str;
        }
    },

    attributes: {
        length: {
            get: function () {
                return this.array.length;
            }
        }
    }
});

