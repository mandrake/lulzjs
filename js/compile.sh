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

patch -fs < tracemonkey-lulzjs.patch 

(autoconf-2.13 || autoconf2.13 || autoconf213) &> /dev/null

CXXFLAGS=-Os ./configure --with-system-nspr --enable-cpp-exceptions --enable-threadsafe --bindir=/usr/bin --libdir=/usr/lib --includedir=/usr/include --enable-debug --enable-gczeal
make || bawww

patch -Rfs < tracemonkey-lulzjs.patch
