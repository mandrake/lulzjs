#! /bin/sh
mkdir -p /usr/include/js
mkdir -p /usr/lib

if [ ! -x $(which autoconf-2.13) ]; then
    echo "Install autoconf 2.13 kthx"
    exit 1
fi

autoconf-2.13
CXXFLAGS=-Os ./configure --enable-thread --bindir=/usr/bin --libdir=/usr/lib --includedir=/usr/include
make
make install

gcc `js-config --cflags` -DXP_UNIX -DJS_THREADSAFE -fPIC -c lulzjs/lulzjs.c -o lulzjs.lo
gcc `js-config --libs` -shared -Wl,-soname,liblulzjs.so -o liblulzjs.so lulzjs.lo -lc
cp -f liblulzjs.so /usr/lib/
cp -f lulzjs/lulzjs.h /usr/include/js
rm lulzjs.lo
rm liblulzjs.so

chmod a+rx /usr/include/js
chmod -R a+r  /usr/include/js
chmod a+rx /usr/lib/liblulzjs.so

