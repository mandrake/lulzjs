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

Object.extend(String, {
    toCharCode: function (str) {
        return str.charCodeAt(0);
    }
});

Object.extend(String.prototype, {
    capitalizeAll: function (options) {
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
    },

    format: function(template) {
        var formatted = this;
        
        for (var i in template) {
            if (typeof(template[i]) != 'function') {
                formatted = formatted.replace(new RegExp("\\{"+i+"\\}", 'g'), template[i]);
            }
        }
    
        return formatted;
    },

    reverse: function () {
        return this.split('').reverse().join('');
    },

    toInt: function (base) {
        return (base)
            ? parseInt(this, base)
            : parseInt(this);
    },

    toFloat: function () {
        return parseFloat(this);
    }
});

