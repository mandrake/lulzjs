#! /bin/sh

function bawww {
    echo ""
    echo "Something went wrong ;_;"
    echo ""
    echo "BAWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW"
    exit 1
}

if [ ! -x $(which autoconf-2.13 &> /dev/null) ] && [ ! -x $(which autoconf2.13 &> /dev/null) ] && [ ! -x $(which autoconf213 &> /dev/null) ]; then
    echo "Install autoconf 2.13 kthx"
    exit 1
fi

(autoconf-2.13 || autoconf2.13 || autoconf213) &> /dev/null

CXXFLAGS=-Os ./configure --with-system-nspr --enable-threadsafe --bindir=/usr/bin --libdir=/usr/lib --includedir=/usr/include --enable-debug --enable-gczeal
make || bawww
make install || bawww

g++ `js-config --cflags` -fPIC -c lulzjs/lulzjs.cpp -o lulzjs.lo -g3 -Wall
g++ `js-config --libs | sed 's/-dynamiclib.*/-dynamiclib -lm/'` -shared -o liblulzjs.so lulzjs.lo -lc || bawww
cp -f liblulzjs.so /usr/lib/ || bawww
cp -f lulzjs/lulzjs.h /usr/include/js || bawww
rm lulzjs.lo || bawww
rm liblulzjs.so || bawww

chmod a+rx /usr/include/js || bawww
chmod -R a+r  /usr/include/js || bawww
chmod a+rx /usr/lib/liblulzjs.so || bawww

