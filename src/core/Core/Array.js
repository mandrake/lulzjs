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

$A = Object.toArray;

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

    function remove (index) {
        return delete this[index];
    };

    function clone () {
        return slice.call(this, 0);
    };

    function inspect () {
        return '[' + this.map(Object.inspect).join(", ") + ']';
    };

    Object.extend(arrayProto, Enumerable, Object.Flags.None);

    Object.extend(arrayProto, {
        _each:     _each,
        clear:     clear,
        compact:   compact,
        flatten:   flatten,
        without:   without,
        uniq:      uniq,
        intersect: intersect,
        remove:    remove,
        clone:     clone,
        toArray:   clone,
        inspect:   inspect,
        toJSON:    toJSON
    }, Object.Flags.None);

})();

Array.addAttributes((function() {
    var size = { get: function size () {
        return this.length;
    }};

    var first = { get: function first () {
        return this[0];
    }};

    var last = { get: function last () {
        return this[this.length - 1];
    }};

    return {
        size:  size,
        first: first,
        last:  last,
    };
})(), Object.Flags.None);

Array.prototype.__iterator__ = function () {
    for (var i = 0; i < this.length; i++) {
        yield i;
    }
}
