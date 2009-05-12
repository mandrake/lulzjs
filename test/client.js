#! /usr/bin/env ljs
require("System/Console");
require("System/Network/Sockets");

var socket = new Sockets.TCP;
socket.connect("localhost:2707");

socket.writeLine("NIGGER");
Console.writeLine("Server: "+socket.readLine());
