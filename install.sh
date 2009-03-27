#! /bin/bash

function bawww {
    echo ""
    echo "Something went wrong ;_;"
    echo ""
    echo "BAWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW"
    exit 1
}

# headers
mkdir -p /usr/include/lulzjs || bawww
cp -rf to_include/* /usr/include/lulzjs || bawww
chmod    a+x  /usr/include/lulzjs || bawww
chmod -R a+r  /usr/include/lulzjs || bawww

# library
cp -f liblulzjs.so /usr/lib/ || bawww
chmod  a+rx /usr/lib/liblulzjs.so || bawww

# config
cp -f js-config /usr/bin/lulzjs-config || bawww
chmod a+xr /usr/bin/lulzjs-config || bawww

echo "Installed tracemonkey without any problem <3"
