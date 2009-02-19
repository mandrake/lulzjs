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

var PeriodicalExecuter = Class.create({
    constructor: function (callback, frequency) {
        this.callback  = callback;
        this.frequency = frequency;
        this.executing = false;

        this.registerCallback();
    },

    methods: {
        registerCallback: function () {
            this.timer = setInterval(this.onTimerEvent.bind(this), this.frequency * 1000);
        },

        execute: function () {
            this.callback(this);
        },

        stop: function () {
            if (!this.timer) {
                return;
            }

            try {
                this.executing = true;
                this.execute();
            }
            finally {
                this.executing = false;
            }
        },

        onTimerEvent: function () {
            if (!this.executing) {
                try {
                    this.executing = true;
                    this.execute();
                }
                finally {
                    this.executing = false;
                }
            }
        }
    }
});

