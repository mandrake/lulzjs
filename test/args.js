#! /usr/bin/ljs
require("System/Console");

Console.writeLine("Program.name: "+this.name);

arguments.forEach(function (value, index) {
    Console.writeLine("{index}: {value}".format({
        index: index,
        value: value
    }));
});
