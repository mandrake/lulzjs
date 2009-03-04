#! /usr/bin/env ljs
require("System/Console");
require("System/Net/Socket");

var socket = new Socket;
socket.listen(null, 2707);

var client = socket.accept();
Console.writeLine("Client: "+client.read(4).toBytes().inspect());
client.write(new Bytes([0x61, 0x00]));
Console.writeLine("DONE :O");
