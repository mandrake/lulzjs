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

function $A (iterable) {
    if (!iterable) {
        return [];
    }
    
    if (iterable.toArray) {
        return iterable.toArray();
    }

    var length  = iteralble.length || 0;
    var results = new Array(length);

    while (length--) {
        results[length] = iterable[length];
    }

    return results;
}

function $w (string) {
    if (!Object.is(string, String)) {
        return [];
    }

    string = string.trim();
    return string ? string.split(/\s+/) : [];
}

Array.__defineProperty__("from", $A, Object.Flags.None);

(function() {
    var arrayProto = Array.prototype;
    var slice      = arrayProto.slice;
    var _each      = arrayProto.forEach;

    function clear () {
        this.length = 0;
        return this;
    };

    function first () {
        return this[0];
    };

    function last () {
        return this[this.length - 1];
    };

    function compact () {
        return [ok for each (ok in this) if (ok)];
    };

    function flatten () {
        return this.inject([], function (array, value) {
            if (Object.is(value, Array)) {
                return array.concat(value.flatten());
            }

            array.push(value);
            return array;
        });
    };

    function without () {
        var values = slice.call(arguments, 0);
        
        return this.select(function (value) {
            return !values.include(value);
        });
    };

    function uniq (sorted) {
        return this.inject([], function (array, value, index) {
            if (index == 0 || (sorted ? array.last() != value : !array.include(value))) {
                array.push(value);
            }

            return array;
        });
    };

    function intersect (array) {
        return this.uniq().findAll(function (item) {
            return array.detect(function (value) {
                return item === value;
            });
        });
    };

    function clone () {
        return slice.call(this, 0);
    };

    function size () {
        return this.length;
    };

    function toJSON () {
        var results = [];

        this.each(function (object) {
            var value = Object.toJSON(object);
            if (value !== undefined) {
                results.push(value);
            }
        });

        return '[' + results.join(", ") + ']';
    };

    Object.extend(arrayProto, Enumerable, Object.Flags.None);

    Object.extend(arrayProto, {
        _each:     _each,
        clear:     clear,
        first:     first,
        last:      last,
        compact:   compact,
        flatten:   flatten,
        without:   without,
        reduce:    reduce,
        uniq:      uniq,
        intersect: intersect
        clone:     clone,
        toArray:   clone,
        inspect:   inspect,
        toJSON:    toJSON
    }, Object.Flags.None);

    arrayProto.__defineGetter__("size", size);
})();

