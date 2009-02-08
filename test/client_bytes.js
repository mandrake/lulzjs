#! /usr/bin/env ljs
require("System/Console");
require("System/Net/Socket");

var socket = new Socket;
socket.connect("localhost", 2707);

socket.sendBytes(new Bytes([0x23,0x42,0x66,0x60]));
var bytes = socket.receiveBytes(2);
Console.writeLine("Server: "+bytes.inspect());

