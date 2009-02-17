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
            method : "GET",
            contentType: "application/x-www-form-urlencoded",
            port   : System.Net.Ports.HTTP,
            timeout: 10,
            ssl    : false,
            requestHeaders: {}
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

        this.response = this._initializeConnection[this.options.method.toUpperCase()].apply(this);
    },

    methods: {
        receive: function (length) {
            if (this.options.method.toUpperCase() == "HEAD") {
                return null;
            }

            var content;
            if (this.response.headers["Content-Length"]) {
                if (this.response.content.length == this.response.headers["Content-Length"].toInt()) {
                    return null;
                }
    
                if (this.response.headers["Content-Type"].match(/^text/)) {
                    return this.response.content += this.socket.receive(length
                        ? length
                        : this.response.headers["Content-Length"].toInt()
                    );
                }
                else {
                    return this.response.content.append(this.socket.receiveBytes(length
                        ? length
                        : this.response.headers["Content-Length"].toInt())
                    );
                }
            }
            else {
                if (this.response.headers["Transfer-Encoding"] == "chunked") {
                    let tmp = this.readChunked(length);
    
                    if (!tmp) {
                        return null;
                    }
                    
                    if (this.response.headers["Content-Type"].match(/^text/)) {
                        return this.response.content += tmp.toString();
                    }
                    else {
                        return this.response.content.append(tmp)
                    }
                }
            }
        },
    
        _initializeConnection: {
            GET: function () {
                this.socket.sendLine([
                    "GET {0} HTTP/1.1".format([this.options.page]),
                    "Host: {0}".format([this.options.host]),
                ].concat(this.getRequestHeadersArray()).concat([""]));
            
                var answer = System.Net.Protocol.HTTP.parseResponse(this.socket.receiveLine());
    
                var headers = '';
                var line;
                while (line = this.socket.receiveLine()) {
                    headers += line+"\n";
                }
                headers = System.Net.Protocol.HTTP.parseHeaders(headers);

                var content = (headers["Content-Type"].match(/^text/)
                    ? new String
                    : new Bytes);
    
                return new System.Net.Protocol.HTTP.Response(answer, headers, content);
            },
    
            POST: function () {
                var params = System.Net.Protocol.HTTP.getTextParams(this.options.params);
        
                this.socket.sendLine([
                    "POST {0} HTTP/1.1".format([this.options.page]),
                    "Host: {0}".format([this.options.host]),
                ].concat(this.getRequestHeadersArray()).concat(["Content-Length: "+params.length, "", params]));
        
                var answer = System.Net.Protocol.HTTP.parseResponse(this.socket.receiveLine());
        
                var headers = '';
                var line;
                while (line = this.socket.receiveLine()) {
                    headers += line+"\n";
                }
                headers = System.Net.Protocol.HTTP.parseHeaders(headers);

                var content = (headers["Content-Type"].match(/^text/)
                    ? new String
                    : new Bytes);
        
                return new System.Net.Protocol.HTTP.Response(answer, headers, content);
            },
        
            HEAD: function () {
                this.socket.sendLine([
                    "HEAD {0} HTTP/1.1".format([this.options.page]),
                    "Host: {0}".format([this.options.host]),
                ].concat(this.getRequestHeadersArray()).concat([""]));
        
                var answer = System.Net.Protocol.HTTP.parseResponse(this.socket.receiveLine());
        
                var headers = '';
                var line;
                while (line = this.socket.receiveLine()) {
                    headers += line+"\n";
                }
                headers = System.Net.Protocol.HTTP.parseHeaders(headers);
        
                return new System.Net.Protocol.HTTP.Response(answer, headers);
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
            var text = this.response.headers["Content-Type"].match(/^text/);

            var ret = text ? new String : new Bytes;
    
            if (!length) {
                let read;
                while (read = this.socket.receiveLine().toInt(16)) {
                    if (text) ret += this.socket.receive(read);
                    else      ret.append(this.socket.receiveBytes(read));
                    this.socket.receiveLine()
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
                    this.socket.receiveLine();
                    this.chunk.length = this.socket.receiveLine().toInt(16);
                    this.chunk.read   = 0;
                }
    
                if (length > this.chunk.length) {
                    if (text) ret += this.socket.receive(this.chunk.length);
                    else      ret.append(this.socket.receiveBytes(this.chunk.length));
                    length -= (this.chunk.read = this.chunk.length);
                }
                else {
                    if (text) ret += this.socket.receive(length);
                    else      ret.append(this.socket.receiveBytes(length));
                    this.chunk.read = length;
                    break;
                }
            }
    
            return ret;
        }
    }
});
