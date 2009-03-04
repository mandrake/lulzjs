#! /usr/bin/env ljs
require("System/Console");
require("System/Net/Socket");

var socket = new Socket;
socket.connect("localhost", 2707);

socket.write(new Bytes([0x23, 0x42, 0x66, 0x60]);
Console.writeLine("Server: "+socket.read(2).toBytes().inspect());

