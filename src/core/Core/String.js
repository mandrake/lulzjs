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

Object.extend(String, {
    interpret: function (value) {
        return value == null ? '' : String(value);
    },

    toCharCode: function (str) {
        return str.charCodeAt(0);
    },

    specialChar: {
        '\b': '\\b',
        '\t': '\\t',
        '\n': '\\n',
        '\f': '\\f',
        '\r': '\\r',
        '\\': '\\\\'
    }
}, Object.Flags.None);

Object.extend(String.prototype, (function() {
    function prepareReplacement (replacement) {
        if (Object.is(replacement, Function)) return replacement;

        var template = new Template(replacement);
        return function (match) {
            return template.evaluate(match);
        };
    };

    function evaluate () {
        return Program.eval(this);
    };

    function gsub (pattern, replacement) {
        pattern    = eval(pattern.toString().replace(/\/(.*)g(.*)/, '/$1$2'));
        var result = "";
        var source = this;
        var match;
        replacement = prepareReplacement(replacement);

        if (!(pattern.length || pattern.source)) {
            replacement = replacement('');
            return replacement + source.split('').join(replacement) + replacement;
        }

        while (source.length > 0) {
            if (match = source.match(pattern)) {
                result += source.slice(0, match.index);
                result += String.interpret(replacement(match));
                source  = source.slice(match.index + match[0].length);
            }
            else {
                result += source;
                source  = "";
            }
        }

        return result;
    };

    function splitEvery (num) {
        var result = new Array;
        var str    = this.toArray();

        for (let i = 0; i < this.length; i += num) {
            result.push(str.slice(i, i+num).join(''));
        }

        return result;
    };

    function sub (pattern, replacement, count) {
        replacement = prepareReplacement(replacement);
        count       = count || 1;

        return this.gsub(pattern, function (match) {
            if (--count < 0) return match[0];
            return replacement(match);
        });
    };

    function test (pattern) {
        return pattern.test(this);
    };

    function scan (pattern, iterator) {
        this.gsub(pattern, iterator);
        return String(this);
    };

    function truncate (length, truncation) {
        length     = length || 30;
        truncation = truncation || "...";

        return this.length > length
            ? this.slice(0, length - truncation.length) + truncation
            : String(this);
    };

    function strip (chars) {
        chars = chars || " ";

        return this.replace(new RegExp(chars.toArray().join('|')), 'g');
    };

    function toArray () {
        return this.split('');
    };

    function next () {
        return this.slice(0, this.length - 1) + String.fromCharCode(
            this.charCodeAt(this.length - 1) + 1
        );
    };

    function times (count) {
        var result = this;

        for (let i = 1; i < count; i++) {
            result += this;
        }

        return result;
    };

    function count (string) {
        var times = 0;
        var check = this;
        var pos;

        while ((pos = check.indexOf(string)) !== -1) {
            check = check.substr(pos+string.length);
            times++;
        }

        return times;
    };

    function inspect (useDoubleQuotes) {
        var escapedString = this.gsub(/[\x00-\x1f\\]/, function(match) {
            var character = String.specialChar[match[0]];
            return character 
                ? character
                : '\\u00' + match[0].charCodeAt().toPaddedString(2, 16);
        });

        if (useDoubleQuotes) {
            return '"' + escapedString.replace(/"/g, '\\"') + '"';
        }
        
        return "'" + escapedString.replace(/'/g, '\\\'') + "'";
    };

    function isJSON () {
        var str = this;

        if (str.blank()) {
            return false;
        }

        str = this.replace(/\\./g, '@').replace(/"[^"\\\n\r]*"/g, '');
        return str.test(/^[,:{}\[\]0-9.\-+Eaeflnr-u \n\r\t]*$/);
    };

    function parseJSON (json, unsecure) {
        return JSON.parse(json, unsecure);
    };

    function include (pattern) {
        return this.indexOf(pattern) > -1;
    };

    function startsWith (pattern) {
        return this.indexOf(pattern) === 0;
    };

    function endsWith (pattern) {
        var d = this.length - pattern.length;
        return d >= 0 && this.lastIndexOf(pattern) === d;
    };

    function empty () {
        return this == '';
    };

    function blank () {
        return /^\s*$/.test(this)
    };

    function interpolate (object, pattern) {
        return new Template(this, pattern).evaluate(object);
    };

    function capitalizeAll (options) {
        options       = options || {}
        var separator = (options.separator || "-_+/\\").toArray();

        var str = this.toArray();

        var capitalize;
        for (var i = 0; i < str.length; i++) {
            if (separator.indexOf(str[i]) != -1) {
                capitalize = true;
                continue;
            }

            if (capitalize) {
                str[i] = str[i].toUpperCase();
                capitalize = false;
            }
        }

        return str.join('');
    };
    
    function commonChars (string) {
        var common = new Array;
        
        for (let i = 0; i < this.length; i++) {
            let char = this.charAt(i);
            if (string.indexOf(char) != -1 && common.indexOf(char) == -1) {
                common.push(char);
            }
        }

        return common;
    };

    function format (template) {
        var formatted = this;
        
        for (var i in template) {
            if (typeof(template[i]) != 'function') {
                formatted = formatted.replace(new RegExp("\\{"+i+"\\}", 'g'), template[i]);
            }
        }
    
        return formatted;
    };

    function reverse () {
        return this.split('').reverse().join('');
    };

    function toNumber () {
        return parseFloat(this);
    };

    function toBytes () {
        return new Bytes(this);
    };

    function toBase (base) {
        return this.toNumber().toBase(base).toUpperCase();
    };

    function fromBase (base) {
        return parseInt(this, base);
    };

    function toCode () {
        return String.toCharCode(this);
    };

    return {
        eval:           evaluate,
        gsub:           gsub,
        sub:            sub,
        test:           test,
        scan:           scan,
        truncate:       truncate,
        strip:          strip,
        toArray:        toArray,
        next:           next,
        splitEvery:     splitEvery,
        times:          times,
        count:          count,
        inspect:        inspect,
        toJSON:         toJSON,
        isJSON:         isJSON,
        parseJSON:      parseJSON,
        include:        include,
        startsWith:     startsWith,
        endsWith:       endsWith,
        empty:          empty,
        blank:          blank,
        interpolate:    interpolate,
        capitalizeAll:  capitalizeAll,
        commonChars:    commonChars,
        format:         format,
        reverse:        reverse,
        toNumber:       toNumber,
        toBytes:        toBytes,
        fromBase:       fromBase,
        toBase:         toBase,
        toCode:         toCode,
    };
})(), Object.Flags.None);

