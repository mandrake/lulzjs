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

Object.extend(Number.prototype, (function() {
    function next () {
        return this + 1;
    };

    function times (iterator, context) {
        $R(0, this, true).each(iterator, context);
        return this;
    };

    function upTo (num, iterator, context) {
        $R(this, num+1, true).each(iterator, context);
        return this;
    };

    function toPaddedString (length, radix) {
        var string = this.toString(radix || 10);
        return '0'.times(length - string.length) + string;
    };

    function isEven () {
        return this % 2 == 0;
    };

    function isOdd () {
        return this % 2 != 0;
    };

    function factorize () {
        var current = this;
        var result  = 1;

        while (current--) {
            result *= current+1;
        }

        return result;
    };

    function abs () {
        return Math.abs(this);
    };

    function round () {
        return Math.round(this);
    };

    function ceil () {
        return Math.ceil(this);
    };

    function floor () {
        return Math.floor(this);
    };

    function log () {
        return Math.log(this);
    };

    function pow (exp) {
        return Math.pow(this, exp);
    };

    function sqrt () {
        return Math.sqrt(this);
    };

    function sin () {
        return Math.sin(this);
    };

    function cos () {
        return Math.cos(this);
    };

    function tan () {
        return Math.tan(this);
    };

    function asin () {
        return Math.asin(this);
    };

    function acos () {
        return Math.acos(this);
    };

    function atan () {
        return Math.atan(this);
    };

    function toBase (base) {
        return this.toString(base).toUpperCase();
    };

    function toChar () {
        return String.fromCharCode(this);
    };

    return {
        next:           next,
        times:          times,
        upTo:           upTo,
        toPaddedString: toPaddedString,
        isEven:         isEven,
        isOdd:          isOdd,
        factorize:      factorize,
        abs:            abs,
        round:          round,
        ceil:           ceil,
        floor:          floor,
        log:            log,
        pow:            pow,
        sqrt:           sqrt,
        sin:            sin,
        cos:            cos,
        tan:            tan,
        asin:           asin,
        acos:           acos,
        atan:           atan,
        toBase:         toBase,
        toChar:         toChar,
    };
})(), Object.Flags.None);

Object.extendAttributes(Number.prototype, {
    digits: { get: function () {
        var matches = this.toString().match(/e(.*)$/);

        if (matches) {
            return (matches[1].toNumber() > 0)
                ? 1+matches[1].toNumber()
                : 0;
        }
        else {
            return this.toString().length;
        }
    }},
}, Object.Flags.None);
