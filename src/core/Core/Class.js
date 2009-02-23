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

Class = {
    create: function () {
        arguments      = Object.toArray(arguments);
        var parent     = (arguments[0].is(Function) ? arguments.shift() : null);
        var properties = arguments.shift();

        if (!properties.is(Object)) {
            throw new Error("You have to pass the class description.");
        }
      
        function klass() {
            if (this.initialize) {
                return this.initialize.apply(this, arguments);
            }

            return null;
        }
    
        Object.extend(klass, Class.Methods);
        klass.superclass = parent;
        klass.subclasses = new Array;
    
        if (parent) {
            if (!parent.subclasses) {
                parent.subclasses = new Array;
            }

            var subclass       = Function.empty.clone();
            subclass.prototype = parent.prototype;
            klass.prototype    = new subclass;
            parent.subclasses.push(klass);
        }

        klass.prototype.initialize = (properties.constructor.is(Function)
            ? properties.constructor
            : Function.empty.clone());
    
        klass.addMethods(properties.methods);
        klass.addStatic(properties.static);
        klass.addAttributes(properties.attributes);
      
        klass.prototype.constructor = klass;
        return klass;
    },

    Methods: {
        addMethods: function (source, flags) {
            flags = (flags !== undefined
                ? flags
                : Object.Flags.None);

            return Object.addMethods(this.prototype, source, flags);
        },

        addStatic: function (source, flags) {
            flags = (flags !== undefined
                ? flags
                : Object.Flags.None);

            return Object.addStatic(this, source, flags);
        },

        addAttributes: function (source, flags) {
            flags = (flags !== undefined
                ? flags
                : Object.Flags.None);

            return Object.addAttributes(this.prototype, source);
        }
    }
};
