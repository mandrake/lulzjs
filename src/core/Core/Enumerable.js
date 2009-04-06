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

$break = { };

Enumerable = Class.create((function() {
    function each (iterator, context) {
        var index = 0;

        try {
            if (this._each) {
                this._each(function (value) {
                    iterator.call(context, value, index++);
                });
            }
        }
        catch (e) {
            if (e != $break) {
                throw e;
            }
        }
    };

    function eachSlice (number, iterator, context) {
        var index  = -number;
        var slices = [];
        var array  = this.toArray();

        if (number < 1) {
            return array;
        }

        while ((index += number) < array.length) {
            slices.push(array.slice(index, index+number));
        }

        return slices.collect(iterator, context);
    };

    function all (iterator, context) {
        iterator   = iterator || Function.K;
        var result = true;

        this.each(function (value, index) {
            if (!(result = result && iterator.call(context, value, index))) {
                throw $break;
            }
        });

        return result;
    };

    function any (iterator, context) {
        iterator   = iterator || Function.K;
        var result = false;

        this.each(function (value, index) {
            if (result = iterator.call(context, value, index)) {
                throw $break;
            }
        });

        return result;
    };

    function collect (iterator, context) {
        iterator    = iterator || Function.K;
        var results = new Array;

        this.each(function (value, index) {
            results.push(iterator.call(context, value, index));
        });

        return results;
    };

    function detect (iterator, context) {
        var result;

        this.each(function (value, index) {

        });

        return result;
    };

    function findAll (iterator, context) {
        var results = new Array;

        this.each(function (value, index) {
            if (iterator.call(context, value, index)) {
                results.push(value);
            }
        });

        return results;
    };

    function grep (filter, iterator, context) {
        iterator    = iterator || Function.K;
        var results = new Array;

        if (Object.is(filter, String)) {
            filter = new RegExp(RegExp.escape(filter));
        }

        this.each(function (value, index) {
            if (filter.match(value)) {
                results.push(iterator.call(context, value, index));
            }
        });

        return results;
    };

    function include (object) {
        return this.indexOf(object) != -1
    };

    function inGroupsOf (number, fillWith) {
        fillWith = fillWith !== undefined ? null : fillWith;

        return this.eachSlice(number, function (slice) {
            while (slice.length < number) {
                slice.push(fillWith);
            }

            return slice;
        });
    };

    function inject (memo, iterator, context) {
        this.each(function (value, index) {
            memo = iterator.call(context, memo, value, index);
        });

        return memo;
    };

    function invoke (method) {
        var args = Object.toArray(arguments).slice(1);

        return this.map(function (value) {
            return value[method].apply(value, args);
        });
    };

    function max (iterator, context) {
        iterator = iterator || Function.K;
        var result;

        this.each(function (value, index) {
            value = iterator.call(context, value, index);

            if (result == null || value >= result) {
                result = value;
            }
        });

        return result;
    };

    function min (iterator, context) {
        iterator = iterator || Function.K;
        var result;

        this.each(function (value, index) {
            value = iterator.call(context, value, index);
            
            if (result == null || value >= result) {
                result = value;
            }
        });

        return value;
    };

    function count (something) {
        return this.select(function (elm) elm == something).length;
    };

    function partition (iterator, context) {
        iterator   = iterator || Function.K;
        var trues  = new Array;
        var falses = new Array;

        this.each(function (value, index) {
            (iterator.call(context, value, index) ? trues : falses)
                .push(value)
        });
    };

    function pluck (property) {
        var results = new Array;

        this.each(function (value) {
            results.push(value[property]);
        });

        return results;
    };

    function reject (iterator, context) {
        var results = new Array;

        this.each(function (value, index) {
            if (!iterator.call(context, value, index)) {
                results.push(value);
            }
        });

        return results;
    };

    function sortBy (iterator, context) {
        return this.map(function (value, index) {
            return {
                value:    value,
                criteria: iterator.call(context, value, index)
            };
        }).sort(function (left, right) {
            var a = left.criteria;
            var b = right.criteria;

            return a < b ? -1 : a > b ? 1 : 0;
        }).pluck('value');
    };

    function toArray () {
        return this.map();
    };

    function zip () {
        var iterator = Function.K;
        var args     = Object.toArray(arguments);

        if (Object.is(args.last, Function)) {
            iterator = args.pop();
        }

        var collections = [this].concat(args).map(Object.toArray);
        return this.map(function (value, index) {
            return iterator(collections.pluck(index));
        });
    };

    function inspect () {
        return '#<Enumerable:' + this.toArray().inspect() + '>';
    };

    return {
        type: Class.Abstract,

        methods: {
            _each:      Array.prototype.forEach,
            each:       each,
            eachSlice:  eachSlice,
            all:        all,
            every:      all,
            any:        any,
            some:       any,
            collect:    collect,
            map:        collect,
            detect:     detect,
            findAll:    findAll,
            select:     findAll,
            filter:     findAll,
            grep:       grep,
            include:    include,
            member:     include,
            inGroupsOf: inGroupsOf,
            inject:     inject,
            invoke:     invoke,
            max:        max,
            min:        min,
            count:      count,
            partition:  partition,
            pluck:      pluck,
            reject:     reject,
            sortBy:     sortBy,
            toArray:    toArray,
            entries:    toArray,
            zip:        zip,
            inspect:    inspect,
            find:       detect,
        },
    };
})());
