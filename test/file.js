#! /usr/bin/ljs
require("System/Console");
require("System/FileSystem/File");

Console.writeLine(new File("file.js", File.Mode.Read).readToEnd());
