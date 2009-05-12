#! /usr/bin/env ljs
require("System/Console");
require("System/Network/Sockets");

var socket = new Sockets.TCP;
socket.listen(null, 2707);
var client = socket.accept();

Console.writeLine("Client: "+client.readLine());
client.writeLine("NO U");
