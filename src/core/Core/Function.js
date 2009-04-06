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

/*. Function < Object {Core}
**
**| The Function object 
*/

Object.extend(Function, {
    empty: function () {},
    K:     function (x) x
}, Object.Flags.None);

Object.extend(Function.prototype, (function() {
    var slice = Array.prototype.slice;

    function update (array, args) {
        var arrayLength = array.length;
        var length      = args.length;

        while (length--) {
            array[arrayLength + length] = args[length];
        }

        return array;
    };

    function merge (array, args) {
        array = slice.call(array, 0);
        return update(array, args);
    };

    function argumentNames () {
        var names = this.toString().match(/\((.*?)\)/)[1].split(", ");

        return names.length == 1 && !names[0] ? [] : names;
    };

    function bind (context) {
        if (arguments.length < 2 && arguments[0] === undefined) return this;

        var __method = this;
        var args   = slice.call(arguments, 1);

        return function () {
            return __method.apply(context, merge(args, arguments));
        }
    };

    function curry () {
        if (!arguments.length) return this;

        var __method = this;
        var args   = slice.call(arguments, 0);

        return function () {
            return __method.apply(this, merge(args, arguments));
        }
    };

    function delay (timeout) {
        var __method  = this;
        var args    = slice.call(arguments, 1);
        timeout    *= 1000;

        return Thread.setTimeout(function() {
            return __method.apply(method, args);
        }, timeout);
    };

    function defer () {
        var args = update([0.01], arguments);
        return this.delay.apply(this, args);
    };

    function wrap (wrapper) {
        var method = this;

        return function () {
            return wrapper.apply(this, update([method.bind(this)], arguments));
        }
    };

    function methodize () {
        if (this._methodized) return this._methodized;

        var __method = this;

        return this._methodized = function () {
            return __method.apply(null, update([this], arguments));
        };
    };

    function clone () {
        return eval(this.toString().replace(/function .*?\(/, 'function ('));
    };

    return {
        argumentNames: argumentNames,
        bind         : bind,
        curry        : curry,
        delay        : delay,
        defer        : defer,
        wrap         : wrap,
        methodize    : methodize,
        clone        : clone
    };
})(), Object.Flags.None);

Object.extend(Function.prototype, Class.Methods, Object.Flags.None);

