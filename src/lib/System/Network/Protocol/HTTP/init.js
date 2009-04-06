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

require([
    "System/System.so",
        "System/Network/Network.so",
            "System/Network/Sockets/Sockets.so", "System/Network/Sockets/Sockets.js",
                "System/Network/Sockets/TCP.so", "System/Network/Sockets/TCP.js",
                
            "System/Network/Ports/Ports.js",

            "System/Network/Protocol/Protocol.so",
                "HTTP.so", "HTTP.js", "Request.js", "Response.js", "Client.js",
]);

if (!Program.HTTP) {
    Program.HTTP = new Object;
}

Object.extend(Program.HTTP, Program.System.Network.Protocol.HTTP);
