#! /usr/bin/env ljs
require("System/Console");
require("System/Net/Protocol/HTTP/Simple");
require("System/FileSystem");
require("System/FileSystem/File");

var url = arguments.shift();
var out = arguments.shift();

var content = HTTP.Get(url);

if (out) {
    Console.writeLine(content);
}
else {
    new File(FileSystem.baseName(url), File.Mode.Write).write(content);
}
