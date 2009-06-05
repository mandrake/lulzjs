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

Math.Parser.Monomial = Class.create({
    constructor: function (str) {
        this.parsed = Monomial.parse(str);

        this.numeric     = this.parsed[0];
        this.literal     = this.parsed[1];
    },

    methods: {
        add: function (m) {
        },
        hasVariable: function (variable) {
            return this.literal.indexOf(variable) != -1;
        }
    },

    static: {
        match: function (str) {
            return new Equation(str).parsed;
        },
        parse: function (str) {
            parsed = new Array;
            index = 0;

            if (str.charAt(0) == '-') {
                negative = true;
                index++;
            }
            else if (str.charAt(0) == '+') {
                negative = false;
                index++;
            }
            else {
                negative = false;
            }

            // Parsing numeric part of the monomial
            while (str.charAt(index) >= '0' && str.charAt(index) <= '9') {
                index++;
            }
            progressive = index;

            // Pushing numeric part
            if (str.charAt(0) == '+' || str.charAt(0) == '-') {
                numeric = str.substring(1, index);
            }
            else {
                numeric = str.substring(0, index);
            }

            if ( numeric != '' ) {
                negative ? parsed.push(-1*(numeric.toNumber())) : parsed.push(numeric.toNumber());
            }
            else {
                negative ? parsed.push(-1) : parsed.push(1);
            }

            parsed.push( new Object );

            // Parsing literal part
            while (str.charAt(index).toLowerCase() >= 'a' && str.charAt(index).toLowerCase() <= 'd'
                || str.charAt(index).toLowerCase() >= 'w' && str.charAt(index).toLowerCase() <= 'z') {
                varIndex = index;
                index++;
                // Checking for exponents
                if (str.charAt(index) == '^') {
                    validExponent = false;
                    index++;

                    if (str.charAt(index) == '+' || str.charAt(index) == '-') {
                        index++;
                    }

                    while (str.charAt(index) >= '0' && str.charAt(index) <= '9') {
                        startExponent = index;
                        index++;
                        validExponent = true;
                    }
                    endExponent = index;

                    if (str.charAt(startExponent-1) == '+' || str.charAt(startExponent-1) == '-') {
                        startExponent--;
                    }

                    if (!validExponent) {
                        throw new Error ("Unknown character in position " + (index+1));
                    }

                    parsed[1][str.charAt(varIndex)] = str.substring(startExponent, endExponent).toNumber();
                }
                // Setting default exponent (1)
                else {
                    parsed[1][str.charAt(varIndex)] = 1;
                }
            }

            // Checking errors
            if (index < str.length) {
                throw new Error ("Unknown character in position " + (index+1));
            }
            return parsed;
        }
    }
});
