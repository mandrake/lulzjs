#! /usr/bin/env ljs

var Person = Class.create({
    constructor: function (name, age) {
        this._name = name;
        this._age  = age;
    },

    methods: {
        
    },

    static: {
        faggotree: function (person) {
            return person.name.charCodeAt(0);
        }
    },

    attributes: {
        name: {
            get: function () { return this._name },
        },

        age: {
            get: function ()    { return this._age },
            set: function (val) { 
                if (val > 1 && val < 125) {
                    this._age = val
                }
            }
        }
    }
});

var jack = new Person("Jack");
print(jack.name);
print(Person.faggotree(jack));
