#! /usr/bin/env ljs

var Animal = Class.create({ type: Class.Abstract,
    constructor: function (name, sound) {
        this._name  = name;
        this._sound = sound;
    },

    methods: {
        speak: function () {
            print("{0} says: {1}!".format([this._name, this._sound]));
        },
    },
});

var Snake = Class.create(Animal, {
    constructor: function ($super, name) {
        $super(name, 'hissssssssss');
    },

    static: {
        eats: ["Babies"]
    },
});

var Nigger = Class.create(Animal, {
    constructor: function ($super) {
        $super("Dix", "WHARS MAH WUTERMALON, FRIED CHIKKIN!");
    },

    static: {
        eats: ["Wutermalon", "Babies"],
    }
});

var nig = new Snake("nig");
nig.speak();

var nog = new Nigger;
nog.speak();
