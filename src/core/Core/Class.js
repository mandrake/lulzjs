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
        var parent     = (Object.is(arguments[0], Function) ? arguments.shift() : null);
        var properties = arguments.shift();

        if (Object.is(properties[0], Function)) {
            parent = properties.shift();
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
            var subclass       = Function.empty.clone();
            subclass.prototype = parent.prototype;
            klass.prototype    = new subclass;
            parent.subclasses.push(klass);
        }

        klass.prototype.initialize = (Object.is(properties.constructor, Function)
            ? properties.constructor
            : Function.empty.clone());
    
        klass.addMethods(properties.methods);
        klass.addStaticMethods(properties.static);
        klass.addAttributes(properties.attributes);
      
        klass.prototype.constructor = klass;
        return klass;
    },

    Methods: {
        addMethods: function (source) {
            if (!source) return;

            var ancestor = this.superclass && this.superclass.prototype;
    
            for (let property in source) {
                let value = source[property];

                if (ancestor && Object.is(value, Function) && value.argumentNames().first() == "$super") {
                    let method = value;

                    value = (function (m) {
                        return function () {
                            return ancestor[m].apply(this, arguments); 
                        };
                    })(property).wrap(method);

                    value.valueOf  = method.valueOf.bind(method);
                    value.toString = method.toString.bind(method);
                }

                this.prototype[property] = value;
            }
        },

        addStaticMethods: function (source) {
            if (!source) return;

            for (let property in source) {
                this[property] = source[property];
            }
    
            return this;
        },

        addAttributes: function (source) {
            if (!source) return;

            for (let attribute in source) {
                if (source[attribute].get) {
                    this.prototype.__defineGetter__(attribute, source[attribute].get);
                }
                if (source[attribute].set) {
                    this.prototype.__defineSetter__(attribute, source[attribute].set);
                }
            }

            return this;
        }
    }
};
