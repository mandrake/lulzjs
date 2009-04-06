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

Template = Class.create({
    type: Class.Normal,

    constructor: function (template, pattern) {
        this.template = template.toString();
        this.pattern  = pattern || Template.Pattern;
    },

    methods: {
        evaluate: function (object) {
            if (Object.is(object.toTemplateReplacements, Function)) {
                object = object.toTemplateReplacements();
            }

            return this.template.gsub(this.pattern, function (match) {
                if (object == null) {
                    return '';
                }

                var before = match[1] || '';
                if (before == '\\') {
                    return match[2];
                }

                var ctx  = object;
                var expr = match[3];

                match = pattern.exec(expr);
                if (match == null) {
                    return before;
                }

                while (match!= null) {
                    var comp = match[1].startsWith('[')
                        ? match[2].gsub('\\\\]', ']')
                        : match[1];

                    ctx = ctx[comp]
                    if (ctx == null || match[3] == '') {
                        break;
                    }

                    expr = expr.substring(
                        '[' == match[3] ? match[1].length : match[0].length
                    );
                    match = pattern.exec(expr);
                }

                return before + String.interpret(ctx);
            });
        }
    },

    static: {
        Pattern: /(^|.|\r|\n)(#\{(.*?)\})/,
    }
});

