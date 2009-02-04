#! /bin/sh
mkdir -p /usr/include/js
mkdir -p /usr/lib

autoconf-2.13 || die "LOL"
./configure --enable-thread --bindir=/usr/bin --libdir=/usr/lib --includedir=/usr/include
make
make install
rm /usr/bin/js

gcc -ljs -I/usr/include/js -DXP_UNIX -DJS_THREADSAFE -fPIC -c lulzjs/lulzjs.c -o lulzjs.lo
gcc -ljs -shared -Wl,-soname,liblulzjs.so -o liblulzjs.so lulzjs.lo -lc
cp -f liblulzjs.so /usr/lib/
cp -f lulzjs/lulzjs.h /usr/include/js
rm lulzjs.lo
rm liblulzjs.so

chmod a+rx /usr/include/js
chmod -R a+r  /usr/include/js
chmod a+rx /usr/lib/libjs.so
chmod a+rx /usr/lib/liblulzjs.so

