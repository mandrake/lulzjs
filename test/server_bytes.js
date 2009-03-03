#! /usr/bin/env ljs
require("System/Console");
require("System/Net/Socket");

var socket = new Socket;
socket.listen(null, 2707);

var client = socket.accept();
var bytes = client.receive(4);
Console.writeLine("Client: "+bytes);
client.send("\x61\x00");
Console.writeLine("DONE :O");
