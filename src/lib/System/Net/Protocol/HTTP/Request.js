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

System.Net.Protocol.HTTP.Request = Class.create({
    constructor: function (url, options) {
        if (!url) {
            throw "You have to pass at least a url.";
        }

        this.options = Object.extend({
            method :        "GET",
            partial:        false,
            contentType:    "application/x-www-form-urlencoded",
            port   :        System.Net.Ports.HTTP,
            timeout:        10,
            ssl    :        false,
            requestHeaders: {},
        }, options);

        this.setDefaultHeaders(this.options.requestHeaders);

        // Implement sockopts for the timeout also ssl stuff.

        var port;
        if (port = /^[^:]*:(\d+)(\/|$)/.exec(url)) {
            this.options.port = port[1];
        }
        else {
            this.options.port = this.options.port;
        }

        if (url.match(/^https/)) {
            this.options.ssl = true;
        }

        var data;
        if (data = /^(http(s)?:\/\/)?([^:\/]*)(:\d+)?(\/.*)?$/.exec(url)) {
            this.options.host = data[3];
            this.options.page = data[5] || "/";
        }
        else {
            throw "The url isn't a valid url, probably.";
        }

        this.socket = new System.Net.Socket;
        if (!this.socket.connect(this.options.host, this.options.port)) {
            throw "Couldn't connect to the host.";
        }

        if (!this.options.partial) {
            this.response = System.Net.Protocol.HTTP.Request
                ._initializeConnection[this.options.method.toUpperCase()].apply(this);
        }
    },

    methods: {
        read: function () {
            if (this.options.partial) {
                return this._partialRead.apply(this, arguments);
            }
            else {
                return this._normalRead.apply(this, arguments);
            }
        },

        _partialRead: function (start, end) {
            if (start === undefined || end === undefined) {
                throw new Error("You have to pass the range.");
            }
            if (end < start) {
                throw new Error("The end has to be larger than the start.");
            }

            if (!this.socket.connected) {
                this.socket = new System.Net.Socket;
                this.socket.connect(this.options.host, this.options.port);
            }

            this.options.requestHeaders["Range"] = "bytes={0}-{1}".format([
                start, end
            ]);

            this.options.requestHeaders["Connection"] = "keep-alive";
            this.options.requestHeaders["Keep-Alive"] = "300";

            this.response = System.Net.Protocol.HTTP.Request
                ._initializeConnection[this.options.method.toUpperCase()].apply(this);

            return this._normalRead(this.response.getHeader("Content-Length").toInt());
        },

        _normalRead: function (length) {
            if (!length) {
                throw new Error("You have to pass a length.");
            }

            if (this.options.method.toUpperCase() == "HEAD") {
                return null;
            }

            var content;
            if (this.response.getHeader("Content-Length")) {
                if (this.response.content.length == this.response.getHeader("Content-Length").toInt()) {
                    return null;
                }
    
                return this.response.content += this.socket.read(length);
            }
            else {
                if (this.response.getHeader("Transfer-Encoding") == "chunked") {
                    let tmp = this.readChunked(length);
    
                    if (!tmp) {
                        return null;
                    }
                    
                    return this.response.content += tmp;
                }
            }
        },

        readToEnd: function () {
            if (this.options.partial) {
                this.response = System.Net.Protocol.HTTP.Request
                    ._initializeConnection[this.options.method.toUpperCase()].apply(this);
            }

            if (this.response.getHeader("Content-Length")) {
                return this.read(this.response.getHeader("Content-Length").toInt())
            }
            else {
                return this.readChunked();
            }
        },

    
        setDefaultHeaders: function (headers) {
            if (Object.is(headers, Array)) {
                if (headers.length % 2) {
                    throw "The array has a odd length.";
                }
    
                var headerz = headers;
                headers     = new Object;
                for (var i = 0; i < headerz.length; i+=2) {
                    headers[headerz[i]] = headerz[i+1];
                }
            }
    
            this.options.requestHeaders = {
                'Connection'  : 'close',
                'User-Agent'  : 'lulzJS/'+__VERSION__,
                'Content-Type': this.options.contentType
            };
    
            Object.extend(this.options.requestHeaders, headers);
        },
    
        getRequestHeadersArray: function () {
            var headerz = [];
    
            for (var header in this.options.requestHeaders) {
                headerz.push("{name}: {value}".format({
                    name: header,
                    value: this.options.requestHeaders[header]
                }));
            }
    
            return headerz;
        },
    
        readChunked: function (length) {
            length = (length === undefined)
                ? 0
                : length;

            var ret = ""

            if (length < 1) {
                while (read = this.socket.readLine().toInt(16)) {
                    ret += this.socket.read(read);
                    this.socket.readLine()
                }
                return ret;
            }
    
            if (!this.chunk) {
                this.chunk = {
                    length: 0,
                    read  : 0
                };
            }
    
            while (this.chunk.length) {
                if (this.chunk.length == this.chunk.read) {
                    this.socket.readLine();
                    this.chunk.length = this.socket.readLine().toInt(16);
                    this.chunk.read   = 0;
                }
    
                if (length > this.chunk.length) {
                    ret += this.socket.read(this.chunk.length);
                    length -= (this.chunk.read = this.chunk.length);
                }
                else {
                    ret += this.socket.read(length);
                    this.chunk.read = length;
                    break;
                }
            }
    
            return ret;
        }
    },

    static: {    
        _initializeConnection: {
            GET: function () {
                this.socket.writeLine([
                    "GET {0} HTTP/1.1".format([this.options.page+(this.options.params ? "?"+this.options.params)]),
                    "Host: {0}".format([this.options.host]),
                ].concat(this.getRequestHeadersArray()).concat([""]));
            
                var answer = System.Net.Protocol.HTTP.parseResponse(this.socket.readLine());
    
                var headers = '';
                var line;
                while (line = this.socket.readLine()) {
                    headers += line+"\n";
                }
                headers = System.Net.Protocol.HTTP.parseHeaders(headers);

                if (!headers["Content-Type"]) {
                    headers["Content-Type"] = "text/plain";
                }

                return new System.Net.Protocol.HTTP.Response(answer, headers, "");
            },
    
            POST: function () {
                var params = System.Net.Protocol.HTTP.getTextParams(this.options.params);
        
                this.socket.writeLine([
                    "POST {0} HTTP/1.1".format([this.options.page]),
                    "Host: {0}".format([this.options.host]),
                ].concat(this.getRequestHeadersArray()).concat(["Content-Length: "+params.length, "", params]));
        
                var answer = System.Net.Protocol.HTTP.parseResponse(this.socket.readLine());
        
                var headers = '';
                var line;
                while (line = this.socket.readLine()) {
                    headers += line+"\n";
                }
                headers = System.Net.Protocol.HTTP.parseHeaders(headers);

                if (!headers["Content-Type"]) {
                    headers["Content-Type"] = "text/plain";
                }

                return new System.Net.Protocol.HTTP.Response(answer, headers, "");
            },
        
            HEAD: function () {
                this.socket.writeLine([
                    "HEAD {0} HTTP/1.1".format([this.options.page]),
                    "Host: {0}".format([this.options.host]),
                ].concat(this.getRequestHeadersArray()).concat([""]));
        
                var answer = System.Net.Protocol.HTTP.parseResponse(this.socket.readLine());
        
                var headers = '';
                var line;
                while (line = this.socket.readLine()) {
                    headers += line+"\n";
                }
                headers = System.Net.Protocol.HTTP.parseHeaders(headers);
        
                return new System.Net.Protocol.HTTP.Response(answer, headers);
            }
        },
    }
});
