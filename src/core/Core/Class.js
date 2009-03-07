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
    Normal:   0x01,
    Abstract: 0x02,

    create: function () {
        arguments      = Object.toArray(arguments);
        var parent     = (arguments[0].is(Function) ? arguments.shift() : null);
        var properties = arguments.shift();

        if (!properties.is(Object)) {
            throw new Error("You have to pass the class description.");
        }
      
        var klass = function () {
            switch (this.__type__) {
                case Class.Abstract:
                throw new Error("You can't instantiate an abstract class.");
                break;

                case Class.Normal:
                if (this.initialize) {
                    return this.initialize.apply(this, arguments);
                }
                break;
            }

            return null;
        }

        Object.extend(klass, Class.Methods, Object.Flags.None);
        klass.__defineProperty__("prototype", klass.prototype);
        klass.__defineProperty__("superclass", parent);
        klass.__defineProperty__("subclasses", new Array);
    
        if (parent) {
            if (!parent.__type__) {
                parent.__type__ = Class.Normal;
            }

            if (!parent.subclasses) {
                parent.__defineProperty__("subclasses", new Array);
            }

            var subclass = Function.empty.clone();
            subclass.__defineProperty__("prototype", parent.prototype);
            klass.__defineProperty__("prototype", new subclass);
            parent.subclasses.push(klass);
        }

        klass.__defineProperty__("__type__",           properties.type || Class.Normal);
        klass.prototype.__defineProperty__("__type__", properties.type || Class.Normal);
        
        var constructor = properties.constructor;
        if (constructor) {
            if (parent && constructor.argumentNames().first() == "$super") {
                constructor = (function (m) {
                    return function () {
                        return parent.prototype[m].apply(this, arguments); 
                    };
                })("initialize").wrap(constructor);
    
                constructor.__defineProperty__("valueOf",  constructor.valueOf.bind(constructor));
                constructor.__defineProperty__("toString", constructor.toString.bind(constructor));
            }
        }
        else {
            constructor = Function.empty.clone();
        }
        

        klass.prototype.__defineProperty__("initialize", constructor);

        if (this.__type__ == Class.Abstract) {
            klass.addStatic(properties.methods);
        }
        else {
            klass.addMethods(properties.methods);
        }

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

            if (this.__type__ == Class.Abstract) {
                Object.addStatic(this, source, flags | Object.Flags.Enumerate);
            }

            return Object.addMethods(this, source, flags);
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

            if (this.__type__ == Class.Abstract) {
                Object.addAttributes(this, source, flags | Object.Flags.Enumerate);
            }
            
            return Object.addAttributes(this.prototype, source, flags);   
        }
    }
};
