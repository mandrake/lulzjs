#! /bin/sh
mkdir -p /usr/include/js
mkdir -p /usr/lib

if [ ! -x $(which autoconf-2.13) ] && [ ! -x $(which autoconf2.13) ]; then
    echo "Install autoconf 2.13 kthx"
    exit 1
fi

autoconf-2.13 || autoconf2.13 &> /dev/null

CXXFLAGS=-Os ./configure --with-system-nspr --enable-threadsafe --bindir=/usr/bin --libdir=/usr/lib --includedir=/usr/include --enable-debug --enable-gczeal
make
make install

g++ `js-config --cflags` -fPIC -c lulzjs/lulzjs.cpp -o lulzjs.lo -g3 -Wall
g++ `js-config --libs` -shared -Wl,-soname,liblulzjs.so -o liblulzjs.so lulzjs.lo -lc
cp -f liblulzjs.so /usr/lib/
cp -f lulzjs/lulzjs.h /usr/include/js
rm lulzjs.lo
rm liblulzjs.so

chmod a+rx /usr/include/js
chmod -R a+r  /usr/include/js
chmod a+rx /usr/lib/liblulzjs.so

