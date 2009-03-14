#! /bin/sh

function bawww {
    echo ""
    echo "Something went wrong ;_;"
    echo ""
    echo "BAWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW"
    exit 1
}

FAIL=true;
which autoconf-2.13 &> /dev/null; if [ !$? ]; then FAIL=false; fi;
which autoconf2.13  &> /dev/null; if [ !$? ]; then FAIL=false; fi;
which autoconf213   &> /dev/null; if [ !$? ]; then FAIL=false; fi;

if $FAIL; then
    echo "Install autoconf 2.13 kthx"
    exit 1
fi

mkdir -p /usr/include/lulzjs/js
mkdir -p /usr/lib/lulzjs

patch js-config.in < js-config.in.lulz.patch

(autoconf-2.13 || autoconf2.13 || autoconf213) &> /dev/null

CXXFLAGS=-Os ./configure --with-system-nspr --enable-threadsafe --bindir=/usr/bin --libdir=/usr/lib/lulzjs --includedir=/usr/include/lulzjs --enable-debug --enable-gczeal
make || bawww
make install || bawww

g++ `js-config --cflags` -fPIC -c lulzjs/lulzjs.cpp -o lulzjs.lo -g3 -Wall
g++ `js-config --libs | sed 's/-dynamiclib.*/-dynamiclib -lm/'` -shared -o liblulzjs.so lulzjs.lo -lc || bawww
cp -f liblulzjs.so /usr/lib/lulzjs/ || bawww
cp -f lulzjs/lulzjs.h /usr/include/lulzjs || bawww
rm lulzjs.lo || bawww
rm liblulzjs.so || bawww

chmod a+rx /usr/include/lulzjs    || bawww
chmod a+rx /usr/include/lulzjs/js || bawww
chmod -R a+r /usr/include/lulzjs/ || bawww
chmod -R a+rx /usr/lib/lulzjs/ || bawww

