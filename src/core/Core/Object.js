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

function $A (iterable) {
}

(function() {
    function getClass (object) {
        return Object.prototype.toString.call(object).match(
            /\[object\s(.*)\]$/)[1];
    };

    function extend (destination, source, flags) {
        flags = (typeof flags == 'number'
            ? flags
            : Object.Flags.Default);

        print(flags);

        for (let property in source) {
            destination.__defineProperty__(property, source[property], flags);
        }

        return destination;
    };

    function inspect (object) {
        try {
            if (object === undefined) return 'undefined';
            if (object === null)      return 'null';

            return object.inspect ? object.inspect() : object.toString();
        }
        catch (e) {
            if (e instanceof RangeError) return '...';
            throw e;
        }
    };

    function toJSON (object) {
        switch (typeof object) {
            case 'undefined':
            case 'function' :
            case 'unknown'  : return;
            case 'boolean'  : return object.toString();
        }

        if (object === null) return 'null';
        if (object.toJSON)   return object.toJSON();

        var results = new Array;
        for (let property in object) {
            let value = toJSON(object[property]);

            if (value !== undefined) {
                results.push(property.toJSON() + ": " + value);
            }
        }

        return '{' + results.join(", ") + '}';
    };

    function keys (object) {
        return [key for (key in object)];
    };

    function values (object) {
        return [value for each (value in object)];
    };

    function clone (object, deep) {
        deep        = deep || false;
        returnValue = new Object;

        if (deep) {
            for (let key in object) {
                returnValue[key] = object[key].clone 
                    ? object[key].clone()
                    : object[key];
            }
        }
        else {
            extend(returnValue, object);
        }

        return returnValue;
    };

    function is (obj, type) {
        try {
            if (typeof type == 'string') {
                if (type.trim().match(/;|\(.*\)$/)) {
                    throw "LOL HAX";
                }

                type = eval(type);
            }

            return obj instanceof type;
        }
        catch (e) {
            return false;
        }
    };

    function toArray (obj) {
        if (!obj) {
            return [];
        }
        
        if (obj.toArray) {
            return obj.toArray();
        }
    
        var length  = obj.length || 0;
        var results = new Array(length);
    
        while (length--) {
            results[length] = obj[length];
        }
    
        return results;
    };

    function addMethods (object, source, flags) {
        if (!object || !source) return;

        flags = flags || Object.Flags.Default;

        var ancestor = object.superclass
            ? object.superclass && object.superclass.prototype
            : undefined;

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

            object.__defineProperty__(property, value, flags);
        }
    };

   function addStatic (object, source, flags) {
        if (!object || !source) return;
        flags = (typeof flags == 'number'
            ? flags
            : Object.Flags.Default);

        for (let property in source) {
            object.__defineProperty__(property, source[property], flags);
        }

        return this;
    };

    function addAttributes (object, source, flags) {
        if (!source) return;
        flags = (typeof flags == 'number'
            ? flags
            : Object.Flags.None);

        for (let attribute in source) {
            object.__defineAttributes__(attribute, source[attribute], flags)
        }

        return this;
    };

    extend(Object, {
        extend :       extend,
        addMethods:    addMethods,
        addStatic:     addStatic,
        addAttributes: addAttributes,
        inspect:       inspect,
        toJSON :       toJSON,
        keys   :       keys,
        values :       values,
        clone  :       clone,
        is     :       is,
        toArray:       toArray
    }, Object.Flags.None);
})();
