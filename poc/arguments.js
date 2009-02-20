#! /usr/bin/env ljs

Program.arguments.parse = function (short, long) {
    if (!short) {
        throw new Error("You have to pass at least the short arguments.");
    }

    var options = new Object;
    var args    = this;

    short = short.toArray();
    for each (let opt in short) {
        for (let n in args) {
            if (args[n][0] == '-' && args[n][1] == opt) {
                if (args[n].length > 2) {
                    options[opt] = args[n].substr(2);
                }
                else {
                }
            }
        }
    }
};
