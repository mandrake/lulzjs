#! /usr/bin/env ljs
require("System/Console");
require("System/Network/Sockets");

var socket = new Sockets.TCP;
socket.connect("localhost:2707");

socket.write(new Bytes([0x23, 0x42, 0x66, 0x60]));
Console.writeLine("Server: "+socket.read(2).toBytes());

