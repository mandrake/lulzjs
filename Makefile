VERSION = 0.1.8

CC         = gcc
CXX        = g++
BINDIR     = /usr/bin
LJS_LIBDIR = /usr/lib/lulzjs
CFLAGS     = -DXP_UNIX -DJS_THREADSAFE -D__LJS_LIBRARY_PATH__="\"${LJS_LIBDIR}\"" -D__LJS_VERSION__="\"${VERSION}\"" -I/usr/include/js -I./src/core -I./src/lib
LDFLAGS    = -ljs -llulzjs

ifdef DEBUG
CFLAGS += -g -DWORKING
endif

ifdef DDEBUG
CFLAGS += -DDEBUG -g
endif

## CORE ##
CORE_DIR     = src/core
CORE         = ${CORE_DIR}/main.o ${CORE_DIR}/Core.o ${CORE_DIR}/Misc.o ${CORE_DIR}/Interactive.o
CORE_CFLAGS  = ${CFLAGS}
CORE_LDFLAGS = ${LDFLAGS} -ldl -lreadline -lncurses

## LIB_CORE ##
LIB_CORE_DIR = src/core/Core
LIB_CORE = \
	${LIB_CORE_DIR}/Base/Thread/Thread.o 

LIB_CORE_CFLAGS  = ${CFLAGS}
LIB_CORE_LDFLAGS = ${LDFLAGS} -lpthread

## LIB_SYSTEM ##

LIB_SYSTEM_DIR = src/lib/System
LIB_SYSTEM = \
	${LIB_SYSTEM_DIR}/System.o \
	${LIB_SYSTEM_DIR}/IO/IO.o ${LIB_SYSTEM_DIR}/IO/Stream/Stream.o ${LIB_SYSTEM_DIR}/IO/File/File.o \
	${LIB_SYSTEM_DIR}/Net/Net.o ${LIB_SYSTEM_DIR}/Net/Socket/Socket.o ${LIB_SYSTEM_DIR}/Net/Protocol/Protocol.o \
	${LIB_SYSTEM_DIR}/Net/Protocol/HTTP/HTTP.o \
	${LIB_SYSTEM_DIR}/Crypt/Crypt.o ${LIB_SYSTEM_DIR}/Crypt/SHA1/SHA1.o

LIB_SYSTEM_CFLAGS  = ${CFLAGS}
LIB_SYSTEM_LDFLAGS = ${LDFLAGS}

all: ljs libcore libsystem

ljs: $(CORE)
	${CC} ${CORE_LDFLAGS} ${CORE_CFLAGS} ${CORE} -o ljs

core_install:
	mkdir -p ${LJS_LIBDIR}
	cp -f ljs ${BINDIR}/

libcore: $(LIB_CORE)

$(LIB_CORE): $(LIB_CORE:.o=.c)
	${CC} ${LIB_CORE_CFLAGS} -fPIC -c $*.c -o $*.lo
	${CC} ${LIB_CORE_LDFLAGS} -shared -Wl,-soname,`basename $*`.so -o $*.o $*.lo -lc

libcore_install: libcore
	mkdir -p ${LJS_LIBDIR}
	mkdir -p ${LJS_LIBDIR}/Core
	mkdir -p ${LJS_LIBDIR}/Core/Prototype
	mkdir -p ${LJS_LIBDIR}/Core/Extension
	mkdir -p ${LJS_LIBDIR}/Core/Base
	mkdir -p ${LJS_LIBDIR}/Core/Base/Thread
########
	cp -f  ${LIB_CORE_DIR}/init.js					${LJS_LIBDIR}/Core/init.js
########
	cp -rf ${LIB_CORE_DIR}/Prototype/*				${LJS_LIBDIR}/Core/Prototype/
########
	cp -rf ${LIB_CORE_DIR}/Extension/*				${LJS_LIBDIR}/Core/Extension/
########
	cp -f  ${LIB_CORE_DIR}/Base/init.js				${LJS_LIBDIR}/Core/Base/init.js
########
	cp -f  ${LIB_CORE_DIR}/Base/Bytes.js			${LJS_LIBDIR}/Core/Base/Bytes.js
########
	cp -f  ${LIB_CORE_DIR}/Base/Thread/init.js		${LJS_LIBDIR}/Core/Base/Thread/init.js
	cp -f  ${LIB_CORE_DIR}/Base/Thread/Thread.o		${LJS_LIBDIR}/Core/Base/Thread/Thread.so
	
libsystem: $(LIB_SYSTEM)

$(LIB_SYSTEM): $(LIB_SYSTEM:.o=.c)
	${CC} ${LIB_SYSTEM_CFLAGS} -fPIC -c $*.c -o $*.lo
	${CC} ${LIB_SYSTEM_LDFLAGS} -shared -Wl,-soname,`basename $*`.so -o $*.o $*.lo -lc

libsystem_install: libsystem
	mkdir -p ${LJS_LIBDIR}
	mkdir -p ${LJS_LIBDIR}/System
	mkdir -p ${LJS_LIBDIR}/System/Console
	mkdir -p ${LJS_LIBDIR}/System/IO
	mkdir -p ${LJS_LIBDIR}/System/IO/Stream
	mkdir -p ${LJS_LIBDIR}/System/IO/File
	mkdir -p ${LJS_LIBDIR}/System/Net
	mkdir -p ${LJS_LIBDIR}/System/Net/Socket
	mkdir -p ${LJS_LIBDIR}/System/Net/Ports
	mkdir -p ${LJS_LIBDIR}/System/Net/Protocol
	mkdir -p ${LJS_LIBDIR}/System/Net/Protocol/HTTP
	mkdir -p ${LJS_LIBDIR}/System/Net/Protocol/HTTP/Simple
	mkdir -p ${LJS_LIBDIR}/System/Crypt
	mkdir -p ${LJS_LIBDIR}/System/Crypt/SHA1
########
	cp -f ${LIB_SYSTEM_DIR}/init.js								${LJS_LIBDIR}/System/init.js
	cp -f ${LIB_SYSTEM_DIR}/System.o							${LJS_LIBDIR}/System/System.so
########
	cp -f ${LIB_SYSTEM_DIR}/IO/init.js							${LJS_LIBDIR}/System/IO/init.js
	cp -f ${LIB_SYSTEM_DIR}/IO/IO.o								${LJS_LIBDIR}/System/IO/IO.so
########
	cp -f ${LIB_SYSTEM_DIR}/IO/Stream/init.js					${LJS_LIBDIR}/System/IO/Stream/init.js
	cp -f ${LIB_SYSTEM_DIR}/IO/Stream/Stream.o					${LJS_LIBDIR}/System/IO/Stream/Stream.so
	cp -f ${LIB_SYSTEM_DIR}/IO/Stream/Stream.js					${LJS_LIBDIR}/System/IO/Stream/Stream.js
########
	cp -f ${LIB_SYSTEM_DIR}/IO/File/init.js						${LJS_LIBDIR}/System/IO/File/init.js
	cp -f ${LIB_SYSTEM_DIR}/IO/File/File.o						${LJS_LIBDIR}/System/IO/File/File.so
	cp -f ${LIB_SYSTEM_DIR}/IO/File/File.js						${LJS_LIBDIR}/System/IO/File/File.js
########
	cp -f ${LIB_SYSTEM_DIR}/Console/init.js						${LJS_LIBDIR}/System/Console/init.js
	cp -f ${LIB_SYSTEM_DIR}/Console/Console.js					${LJS_LIBDIR}/System/Console/Console.js
#######
	cp -f ${LIB_SYSTEM_DIR}/Net/init.js							${LJS_LIBDIR}/System/Net/init.js
	cp -f ${LIB_SYSTEM_DIR}/Net/Net.o							${LJS_LIBDIR}/System/Net/Net.so
#######
	cp -f ${LIB_SYSTEM_DIR}/Net/Socket/init.js					${LJS_LIBDIR}/System/Net/Socket/init.js
	cp -f ${LIB_SYSTEM_DIR}/Net/Socket/Socket.o					${LJS_LIBDIR}/System/Net/Socket/Socket.so
	cp -f ${LIB_SYSTEM_DIR}/Net/Socket/Socket.js				${LJS_LIBDIR}/System/Net/Socket/Socket.js
#######
	cp -f ${LIB_SYSTEM_DIR}/Net/Ports/init.js					${LJS_LIBDIR}/System/Net/Ports/init.js
	cp -f ${LIB_SYSTEM_DIR}/Net/Ports/Ports.js					${LJS_LIBDIR}/System/Net/Ports/Ports.js
#######
	cp -f ${LIB_SYSTEM_DIR}/Net/Protocol/init.js				${LJS_LIBDIR}/System/Net/Protocol/init.js
	cp -f ${LIB_SYSTEM_DIR}/Net/Protocol/Protocol.o				${LJS_LIBDIR}/System/Net/Protocol/Protocol.so
#######
	cp -f ${LIB_SYSTEM_DIR}/Net/Protocol/HTTP/init.js			${LJS_LIBDIR}/System/Net/Protocol/HTTP/init.js
	cp -f ${LIB_SYSTEM_DIR}/Net/Protocol/HTTP/HTTP.o			${LJS_LIBDIR}/System/Net/Protocol/HTTP/HTTP.so
	cp -f ${LIB_SYSTEM_DIR}/Net/Protocol/HTTP/HTTP.js			${LJS_LIBDIR}/System/Net/Protocol/HTTP/HTTP.js
	cp -f ${LIB_SYSTEM_DIR}/Net/Protocol/HTTP/Request.js 		${LJS_LIBDIR}/System/Net/Protocol/HTTP/Request.js
	cp -f ${LIB_SYSTEM_DIR}/Net/Protocol/HTTP/Response.js		${LJS_LIBDIR}/System/Net/Protocol/HTTP/Response.js
	cp -f ${LIB_SYSTEM_DIR}/Net/Protocol/HTTP/Client.js			${LJS_LIBDIR}/System/Net/Protocol/HTTP/Client.js
#######
	cp -f ${LIB_SYSTEM_DIR}/Net/Protocol/HTTP/Simple/init.js	${LJS_LIBDIR}/System/Net/Protocol/HTTP/Simple/init.js
	cp -f ${LIB_SYSTEM_DIR}/Net/Protocol/HTTP/Simple/Simple.js	${LJS_LIBDIR}/System/Net/Protocol/HTTP/Simple/Simple.js
#######
	cp -f ${LIB_SYSTEM_DIR}/Crypt/init.js						${LJS_LIBDIR}/System/Crypt/init.js
	cp -f ${LIB_SYSTEM_DIR}/Crypt/Crypt.o						${LJS_LIBDIR}/System/Crypt/Crypt.so
#######
	cp -f ${LIB_SYSTEM_DIR}/Crypt/SHA1/init.js					${LJS_LIBDIR}/System/Crypt/SHA1/init.js
	cp -f ${LIB_SYSTEM_DIR}/Crypt/SHA1/SHA1.o					${LJS_LIBDIR}/System/Crypt/SHA1/SHA1.so
	cp -f ${LIB_SYSTEM_DIR}/Crypt/SHA1/SHA1.js					${LJS_LIBDIR}/System/Crypt/SHA1/SHA1.js

libsystem_uninstall:

install: all core_install libcore_install libsystem_install
	chmod -R a+rx ${LJS_LIBDIR}
	chmod a+rx ${BINDIR}/ljs

uninstall:
	rm -f ${BINDIR}/ljs
	rm -rf ${LJS_LIBDIR}/Core
	rm -rf ${LJS_LIBDIR}/System
	

clean:
	rm -f ljs;
	find src|egrep "\.l?o"|xargs rm -f

