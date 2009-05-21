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
Math.Economy.VAT.Austria = {
    Standard: 20,
    Reduced:  12,
};

Math.Economy.VAT.Azores = {
    Standard: 15,
    Reduced:  8,
};

Math.Economy.VAT.Belgium = {
    Standard: 21,
    Reduced:  12,
};

Math.Economy.VAT.Bulgaria = {
    Standard: 20,
    Reduced:  7,
};

Math.Economy.VAT.Cyprus = {
    Standard: 15,
    Reduced:  5,
};

Math.Economy.VAT.CzechRepublic = {
    Standard: 19,
    Reduced:  9,
};

Math.Economy.VAT.Denmark = {
    Standard: 25,
    Reduced:  0,
};

Math.Economy.VAT.Estonia = {
    Standard: 18,
    Reduced:  9,
};

Math.Economy.VAT.Finland = {
    Standard: 22,
    Reduced:  17,
};

Math.Economy.VAT.France = {
    Standard: 19.6,
    Reduced:  5.5,
};

Math.Economy.VAT.Germany = {
    Standard: 19,
    Reduced:  9,
};

Math.Economy.VAT.Greece = {
    Standard: 19,
    Reduced:  9,
};

Math.Economy.VAT.Hungary = {
    Standard: 20,
    Reduced:  5,
};

Math.Economy.VAT.Ireland = {
    Standard: 21.5,
    Reduced:  13.5,
};

Math.Economy.VAT.Italy = {
    Standard: 20,
    Reduced:  10,
};

Math.Economy.VAT.Latvia = {
    Standard: 21,
    Reduced:  10,
};

Math.Economy.VAT.Lithuania = {
    Standard: 19,
    Reduced:  9,
};

Math.Economy.VAT.Luxembourg = {
    Standard: 15,
    Reduced:  12,
};

Math.Economy.VAT.Madeira = {
    Standard: 15,
    Reduced:  8,
};

Math.Economy.VAT.Malta = {
    Standard: 18,
    Reduced:  5,
};

Math.Economy.VAT.Netherlands = {
    Standard: 19,
    Reduced:  6,
};

Math.Economy.VAT.Poland = {
    Standard: 22,
    Reduced:  7,
};

Math.Economy.VAT.Portugal = {
    Standard: 20,
    Reduced:  12,
};

Math.Economy.VAT.Romania = {
    Standard: 19,
    Reduced:  9,
};

Math.Economy.VAT.Slovakia = {
    Standard: 19,
    Reduced:  10,
};

Math.Economy.VAT.Slovenia = {
    Standard: 20,
    Reduced:  8.5,
};

Math.Economy.VAT.Spain = {
    Standard: 16,
    Reduced:  7,
};

Math.Economy.VAT.CanaryIslands = {
    Standard: 5,
    Reduced:  2,
};

Math.Economy.VAT.Sweden = {
    Standard: 25,
    Reduced:  12,
};

Math.Economy.VAT.UnitedKingdom = {
    Standard: 15,
    Reduced:  5,
};

Math.Economy.VAT.Albania = {
    Standard: 20,
    Reduced:  0,
};

Math.Economy.VAT.Argentina = {
    Standard: 21,
    Reduced:  10.5,
};

Math.Economy.VAT.Armenia = {
    Standard: 20,
    Reduced:  0,
};

Math.Economy.VAT.Australia = {
    Standard: 10,
    Reduced:  0,
};

Math.Economy.VAT.Belarus = {
    Standard: 0,
    Reduced:  0,
};

Math.Economy.VAT.Barbados = {
    Standard: 15,
    Reduced:  0,
};

Math.Economy.VAT.BosniaHerzegovina = {
    Standard: 17,
    Reduced:  0,
};

Math.Economy.VAT.Brazil = {
    Standard: 12 + 25 + 5,
    Reduced:  0,
};

Math.Economy.VAT.Canada = {
    Standard: 5,
    Reduced:  4.5,
};

Math.Economy.VAT.Chile = {
    Standard: 19,
    Reduced:  0,
};

Math.Economy.VAT.Colombia = {
    Standard: 15,
    Reduced:  0,
};

Math.Economy.VAT.China = {
    Standard: 17,
    Reduced:  6,
};

Math.Economy.VAT.Croatia = {
    Standard: 22,
    Reduced:  10,
};

Math.Economy.VAT.DominicanRepublic = {
    Standard: 16,
    Reduced:  12,
};

Math.Economy.VAT.Ecuador = {
    Standard: 12,
    Reduced:  0,
};

Math.Economy.VAT.Egypt = {
    Standard: 10,
    Reduced:  0,
};

Math.Economy.VAT.ElSalvador = {
    Standard: 13,
    Reduced:  0,
};

Math.Economy.VAT.Fiji = {
    Standard: 12.5,
    Reduced:  0,
};

Math.Economy.VAT.Georgia = {
    Standard: 18,
    Reduced:  0,
};

Math.Economy.VAT.Guatemala = {
    Standard: 12,
    Reduced:  0,
};

Math.Economy.VAT.Guyana = {
    Standard: 16,
    Reduced:  0,
};

Math.Economy.VAT.Iran = {
    Standard: 3,
    Reduced:  0,
};

Math.Economy.VAT.Iceland = {
    Standard: 24.5,
    Reduced:  7,
};

Math.Economy.VAT.India = {
    Standard: 12.5,
    Reduced:  4,
};

Math.Economy.VAT.Indonesia = {
    Standard: 10,
    Reduced:  5,
};

Math.Economy.VAT.Israel = {
    Standard: 15.5,
    Reduced:  0,
};

Math.Economy.VAT.Japan = {
    Standard: 5,
    Reduced:  0,
};

Math.Economy.VAT.SouthKorea = {
    Standard: 10,
    Reduced:  0,
};

Math.Economy.VAT.Jersey = {
    Standard: 3,
    Reduced: 0,
};

Math.Economy.VAT.Jordan = {
    Standard: 16,
    Reduced:  0,
};

// GREAT SUCCESS
Math.Economy.VAT.Kazakhstan = {
    Standard: 13,
    Reduced:  0,
};

Math.Economy.VAT.Kosovo = {
    Standard: 16,
    Reduced:  0,
};

Math.Economy.VAT.Lebanon = {
    Standard: 10,
    Reduced:  0,
};

Math.Economy.VAT.Moldova = {
    Standard: 20,
    Reduced:  5,
};

Math.Economy.VAT.Macedonia = {
    Standard: 18,
    Reduced:  5,
};

Math.Economy.VAT.Malaysia = {
    Standard: 10,
    Reduced:  0,
};

Math.Economy.VAT.Mexico = {
    Standard: 15,
    Reduced:  0,
};

Math.Economy.VAT.Montenegro = {
    Standard: 17,
    Reduced:  0,
};

Math.Economy.VAT.NewZealand = {
    Standard: 12.5,
    Reduced:  0,
};

Math.Economy.VAT.Norway = {
    Standard: 25,
    Reduced:  14,
};

Math.Economy.VAT.Pakistan = {
    Standard: 16,
    Reduced:  1,
};

Math.Economy.VAT.Panama = {
    Standard: 5,
    Reduced:  0,
};

Math.Economy.VAT.Paraguay = {
    Standard: 10,
    Reduced:  5,
};

Math.Economy.VAT.Peru = {
    Standard: 19,
    Reduced:  0,
};

Math.Economy.VAT.Philippines = {
    Standard: 12,
    Reduced:  0,
};

// MOTHERLAND
Math.Economy.VAT.Russia = {
    Standard: 18,
    Reduced:  10,
};

Math.Economy.VAT.Serbia = {
    Standard: 18,
    Reduced:  8,
};

Math.Economy.VAT.Singapore = {
    Standard: 7,
    Reduced:  0,
};

Math.Economy.VAT.SouthAfrica = {
    Standard: 14,
    Reduced:  0,
};

Math.Economy.VAT.SriLanka = {
    Standard: 12,
    Reduced:  0,
};

Math.Economy.VAT.Switzerland = {
    Standard: 7.6,
    Reduced:  3.6,
};

Math.Economy.VAT.Thailand = {
    Standard: 7,
    Reduced: 0,
};

Math.Economy.VAT.TrinidadTobago = {
    Standard: 15,
    Reduced:  0,
};

Math.Economy.VAT.Turkey = {
    Standard: 18,
    Reduced:  8,
};

Math.Economy.VAT.Ukraine = {
    Standard: 20,
    Reduced:  0,
};

Math.Economy.VAT.Uruguay = {
    Standard: 22,
    Reduced:  10,
};

Math.Economy.VAT.Vietnam = {
    Standard: 10,
    Reduced:  5,
};

Math.Economy.VAT.Venezuela = {
    Standard: 12,
    Reduced:  11,
};
