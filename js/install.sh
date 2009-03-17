#! /bin/sh

function bawww {
    echo ""
    echo "Something went wrong ;_;"
    echo ""
    echo "BAWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW"
    exit 1
}

make install || bawww
chmod    a+x  /usr/include/lulzjs
chmod -R a+r  /usr/include/lulzjs
chmod    a+rx /usr/lib/liblulzjs.so

