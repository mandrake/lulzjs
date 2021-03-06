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
        "Network.so",

        "Sockets/Sockets.so", "Sockets/Sockets.js",
            "Sockets/TCP.so",  "Sockets/TCP.js",
            "Sockets/UDP.so",  "Sockets/UDP.js",
            "Sockets/ICMP.so", "Sockets/ICMP.js",
            "Sockets/RAW.so",  "Sockets/RAW.js",

        "Ports/Ports.js"
]);

Network = System.Network;

