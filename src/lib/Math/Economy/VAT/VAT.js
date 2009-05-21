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

Math.Economy.VAT = Class.create({
    constructor: function (vat) {
        this._vat = vat;
    },

    methods: {
        get: function (value) {
            return Math.Economy.VAT.get(value, this._vat);
        },

        apply: function (value) {
            return Math.Economy.VAT.apply(value, this._vat);
        }
    },

    static: {
        get: function (value, vat) {
            return value / 100 * vat;
        },

        apply: function (value, vat) {
            return value + Math.Economy.VAT.get(value, vat);
        },
    },
});

/* States */
Math.Economy.VAT.Italy = {
    Standard: 20,
    Reduced:  12,
};


