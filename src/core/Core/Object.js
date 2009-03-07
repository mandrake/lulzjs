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

(function() {
    function getClass (object) {
        return Object.prototype.toString.call(object).match(
            /\[object\s(.*)\]$/)[1];
    };

    function extend (destination, source, flags, overwrite) {
        flags = (typeof flags == 'number'
            ? flags
            : Object.Flags.Default);

        overwrite = (overwrite === undefined)
            ? true
            : overwrite;

        for (let property in source) {
            if (!overwrite && destination[property] !== undefined) {
                continue;
            }

            destination.__defineProperty__(property, source[property], flags);
        }

        return destination;
    };

    function addMethods (object, source, flags) {
        if (!object || !source) return;

        flags = (flags !== undefined
            ? flags
            : Object.Flags.None);

        for (let property in source) {
            object.prototype.__defineProperty__(property, source[property], flags);
        }
    };

   function addStatic (object, source, flags) {
        if (!object || !source) return;

        flags = (flags !== undefined
            ? flags
            : Object.Flags.None);

        for (let property in source) {
            object.__defineProperty__(property, source[property], flags);
        }

        return this;
    };

    function addAttributes (object, source, flags) {
        if (!source) return;

        flags = (flags !== undefined
            ? flags
            : Object.Flags.None);

        for (let attribute in source) {
            object.__defineAttributes__(attribute, source[attribute], flags)
        }

        return this;
    };

    function inspect (object) {
        try {
            if (object === undefined)   return 'undefined';
            if (object === null)        return 'null';
            if (typeof object == 'xml') return "XML:\n"+object.toXMLString();

            return (object.inspect && object.inspect != Object.prototype.inspect)
                ? object.inspect()
                : object.toString();
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
        
        if (obj.toArray && obj.toArray != Object.prototype.toArray) {
            return obj.toArray();
        }
    
        var length  = obj.length || 0;
        var results = new Array(length);
    
        for (let i = 0; i < length; i++) {
            results[i] = obj[i];
        }
    
        return results;
    };

    extend(Object, {
        getClass:      getClass,   
        extend:        extend,
        addMethods:    addMethods,
        addStatic:     addStatic,
        addAttributes: addAttributes,
        inspect:       inspect,
        toJSON:        toJSON,
        keys:          keys,
        values:        values,
        clone:         clone,
        is:            is,
        toArray:       toArray
    }, Object.Flags.None);
})();

Object.extend(Object.prototype, (function() {
    function extend (source, flags, overwrite) {
        return Object.extend(this, source, flags, overwrite);
    };

    function addMethods (source, flags) {
        Object.addMethods(this, source, flags);
    };

    function addStatic (source, flags) {
        Object.addStatic(this, source, flags);
    };

    function addAttributes (source, flags) {
        Object.addAttributes(this, source, flags);
    };

    function is (type) {
        return Object.is(this, type);
    };

    function toArray () {
        return Object.toArray(this);
    };

    function inspect () {
        return Object.inspect(this);
    };

    function keys () {
        return Object.keys(this);
    };

    function values () {
        return Object.values(this);
    };

    function toJSON () {
        return Object.toJSON(this);
    };

    function clone (deep) {
        return Object.clone(this, deep);
    };

    return {
        extend :       extend,
        addMethods:    addMethods,
        addStatic:     addStatic,
        addAttributes: addAttributes,
        toJSON :       toJSON,
        keys   :       keys,
        values :       values,
        clone  :       clone,
        is     :       is,
        toArray:       toArray,
        inspect:       inspect,
    };
})(), Object.Flags.None);
