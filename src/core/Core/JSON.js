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

/*. JSON < Object {Core}
**
**| The JSON object takes care of parsing and stringifying objects securely
**| so you don't have to worry about evals or other instructions in JSON
**| data.
*/

// Saving default methods.
JSON._parse     = JSON.parse;
JSON._stringify = JSON.stringify;

/*:# parse (json[, unsecure = false]) => Object
**  - json [String]: The JSON to transform into an object.
**  - unsecure [Boolean]: When true the string gets evaluated with some small
**                        security checks. (it has the ability to parse methods too)
**
**| The parse method just transforms JSON text into an Object.
*/
JSON.parse = function (json, unsecure) {
    if (!unsecure) {
        return JSON._parse(json);
    }
    else {
        if (json.isJSON()) {
            return eval('('+json+')');
        }
    }
};

/*:# stringify (obj) => String
**  - obj [Object]: The object to stringify.
**
**| The stringify method transforms to JSON the passed object, if the object has
**| a toJSON method that method is used, if not the default is used.
*/
JSON.stringify = function (obj) {
    return obj.toJSON();
};

