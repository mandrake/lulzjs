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

function $R (start, end, exclusive) {
    return new Range(start, end, exclusive);
};

Range = Class.create(Enumerable, (function() {
    function constructor (start, end, exclusive) {
        this.start     = start;
        this.end       = end;
        this.exclusive = exclusive;
    };

    function _each (iterator) {
        var value = this.start;

        while (this.include(value)) {
            iterator(value);
            value = value.next();
        }
    };

    function include (value) {
        if (value < this.start) {
            return false;
        }
        
        if (this.exclusive) {
            return value < this.end;
        }

        return value <= this.end;
    };

    return {
        type: Class.Normal,

        constructor: constructor,

        methods: {
            _each:   _each,
            include: include
        }
    };
})());
