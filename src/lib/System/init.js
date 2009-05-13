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
    "System.so",
        "Environment/Environment.so",

        "IO/IO.so",
            "IO/Pipe/Pipe.so", "IO/Pipe/Pipe.js",

        "FileSystem/FileSystem.so", "FileSystem/FileSystem.js",
            "FileSystem/File/File.so", "FileSystem/File/File.js",
            "FileSystem/Directory/Directory.so", "FileSystem/Directory/Directory.js",

        "Network/Network.so",
            "Network/Sockets/Sockets.so", "Network/Sockets/Socket.js",
                "Network/Sockets/TCP.so",  "Network/Sockets/TCP.js",
                "Network/Sockets/UDP.so",  "Network/Sockets/UDP.js",
                "Network/Sockets/ICMP.so", "Network/Sockets/ICMP.js",
                "Network/Sockets/RAW.so",  "Network/Sockets/RAW.js",

            "Network/Ports/Ports.js",

            "Network/Protocol/Protocol.so",
                "Network/Protocol/HTTP/HTTP.so", "Network/Protocol/HTTP/HTTP.js",
                    "Network/Protocol/HTTP/Request.js", "Network/Protocol/HTTP/Response.js",
                    "Network/Protocol/HTTP/Client.js", "Network/Protocol/HTTP/Simple/Simple.js",

        "Math/Math.so",
            "Math/Crypt/Crypt.so",
                "Crypt/SHA1/SHA1.so", "Crypt/SHA1/SHA1.js",

        "Console/Console.so", "Console/Console.js",
]);

