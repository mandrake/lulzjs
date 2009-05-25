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

Math.Sequences.Fibonacci = Class.create({
    constructor: function (limit) {
        this.limit     = limit;
        this.generator = new Math.Sequences.Fibonacci.Generator(limit);
    },

    methods: {
        last: function () {
            if (!this.limit) {
                throw new Error("Do you want to kill the CPU?");
            }
            
            for (var n in this.generator);
            this.generator = new Math.Sequences.Fibonacci.Generator(this.limit)
            return n;
        },

    },
    
    static: {
        at: function (index) {
            if (index < 1) {
                throw new Error("The sequence starts from 1.")
            }

            return ((1/(5).sqrt())*((1+(5).sqrt())/2).pow(index) - (1/(5).sqrt()) * ((1-(5).sqrt())/2).pow(index)).ceil();
        },

        Generator: function (limit) {
            var a = 1;
            var b = 1;
        
            while (true) {
                var current = b;
                b           = a;
                a           = a + current;
        
                if (limit && current > limit) {
                    return;
                }
        
                yield current;
            }
        },
    }
});

Math.Sequences.Fibonacci.prototype.__iterator__ = function () {
    return this.generator;
};
