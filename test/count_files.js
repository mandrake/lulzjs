#! /usr/bin/env ljs
require("System/Console");
require("System/FileSystem/Directory");
require("System/FileSystem/File");

files = new Directory(arguments.shift() || ".").toArray();
hash  = new Hash;
total = 0;

for each (let file in files) {
    if (!file.is(File)) {
        continue;
    }

    let ext = file.name.match(/.(\..*?)$/);
    if (!ext) {
        total++;
        continue;
    }
    ext = ext[1]

    if (!hash.get(ext)) {
        hash.set(ext, new Array);
    }

    hash.get(ext).push(file.name);
}

if (hash.length) {
    hash.each(function (pair) {
        total += pair.value.length;
        Console.writeLine("{0} {1} files found.".format([pair.key, pair.value.length]));
    });
    Console.writeLine("");
}

Console.writeLine("{0} total files found.".format([total]));
