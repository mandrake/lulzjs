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

if (!String._crypt) {
    String._crypt          = new Object;
    String._crypt.hashing  = new Object;
    String._crypt.crypting = new Object;
}

Object.extend(Math.Cryptography, {
    addAlgorithm: function (name, type, class) {
        if (type.toLowerCase() == "hash") {
            String._crypt.hashing[name] = class;
        }
        else if (type.toLowerCase() == "crypt") {
            String._crypt.crypting[name] = class;
        }
    },
});

Object.extend(String.prototype, {
    encrypt: function (alg, params) {
        if (String._crypt.crypting[alg]) {
            let parms = "";

            if (params) {
                for (let i = 0; i < params.length; i++) {
                    parms += "params["+i+"], ";
                }
                parms = parms.substr(0, parms.length-2);
            }

            return eval("new String._crypt.crypting[alg](this).encrypt("+parms+")");
        }
        else {
            return null;
        }
    },

    decrypt: function (alg, params) {
        if (String._crypt.crypting[alg]) {
            let parms = "";

            if (params) {
                for (let i = 0; i < params.length; i++) {
                    parms += "params["+i+"], ";
                }
                parms = parms.substr(0, parms.length-2);
            }

            return eval("new String._crypt.crypting[alg](this).decrypt("+parms+")");
        }
        else {
            return null;
        }
    },

    hash: function (alg) {
        if (String._crypt.hashing[alg]) {
            return new String._crypt.hashing[alg](this).toString();
        }
        else {
            return null;
        }
    },
});

