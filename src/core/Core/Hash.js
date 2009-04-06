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

Hash = Class.create(Enumerable, (function() {
    function constructor (object) {
        this._object = Object.is(object, Hash)
            ? object.toObject()
            : Object.clone(object);
    };

    function _each (iterator) {
        for (let key in this._object) {
            let value  = this._object[key];
            let pair   = [key, value];
            pair.key   = key;
            pair.value = value;
            iterator(pair);
        }
    };

    function set (key, value) {
        return this._object[key] = value;
    };

    function get (key) {
        if (this._object[key] !== Object.prototype[key]) {
            return this._object[key];
        }
    };

    function unset (key) {
        var value = this._object[key];
        delete this._object[key];
        return value;
    }

    function toObject () {
        return Object.clone(this._object);
    };

    function keys () {
        return this.pluck('key');
    };

    function values () {
        return this.pluck('value');
    };

    function index (value) {
        var match = this.detect(function (pair) {
            return pair.value === value;
        });

        return match && match.key;
    };

    function merge (object) {
        return this.clone().update(object);
    };

    function update (object) {
        return new Hash(object).inject(this, function (result, pair) {
            result.set(pair.key, pair.value);
            return result;
        });
    };

    function inspect () {
        return "#<Hash:{" + this.map(function (pair) {
            return pair.map(Object.inspect).join(": ");
        }).join(", ") + "}>";
    };

    function toJSON () {
        return Object.toJSON(this.toObject());
    };

    function clone () {
        return new Hash(this);
    };

    return {
        type: Class.Normal,

        constructor: constructor,
        
        methods: {
            _each:                  _each,
            set:                    set,
            get:                    get,
            unset:                  unset,
            toObject:               toObject,
            toTemplateReplacements: toObject,
            keys:                   keys,
            values:                 values,
            index:                  index,
            merge:                  merge,
            update:                 update,
            inspect:                inspect,
            toJSON:                 toJSON,
            clone:                  clone,
        },

        attributes: {
            length: {
                get: function () {
                    return this.keys().length;
                }
            },
        },
    };
})());

function $H (object) {
    return new Hash(object);
};

Hash.from = $H;
