VERSION = 0.1.8

CC         = gcc
CXX        = g++
BINDIR     = /usr/bin
LJS_LIBDIR = /usr/lib/lulzjs
CFLAGS     = -D__LJS_LIBRARY_PATH__="\"${LJS_LIBDIR}\"" -D__LJS_VERSION__="\"${VERSION}\"" $(shell js-config --cflags) -I./src/core -I./src/lib 
LDFLAGS    = $(shell js-config --libs | sed 's/-dynamic.*/-lm/') -llulzjs

ifdef DEBUG
CFLAGS += -g3 -DWORKING -Wall
endif

ifdef GCBOOM
CFLAGS += -DGCBOOM
endif

ifdef DDEBUG
CFLAGS += -DDEBUG -g3 -Wall
endif

## CORE ##
CORE_DIR     = src/core
CORE         = ${CORE_DIR}/main.cpp ${CORE_DIR}/Core.cpp ${CORE_DIR}/Interactive.cpp
CORE_CFLAGS  = ${CFLAGS}
CORE_LDFLAGS = ${LDFLAGS} -ldl -lreadline -lncurses

## LIB_CORE ##
LIB_CORE_DIR = src/core/Core
LIB_CORE = \
	${LIB_CORE_DIR}/Object.o ${LIB_CORE_DIR}/Thread/Thread.o 

LIB_CORE_CFLAGS  = ${CFLAGS}
LIB_CORE_LDFLAGS = ${LDFLAGS}

## LIB_SYSTEM ##

LIB_SYSTEM_DIR = src/lib/System
LIB_SYSTEM = \
	${LIB_SYSTEM_DIR}/System.o \
	${LIB_SYSTEM_DIR}/Environment/Environment.o \
	${LIB_SYSTEM_DIR}/Console/Console.o \
	${LIB_SYSTEM_DIR}/FileSystem/FileSystem.o ${LIB_SYSTEM_DIR}/FileSystem/File/File.o ${LIB_SYSTEM_DIR}/FileSystem/Directory/Directory.o \
	${LIB_SYSTEM_DIR}/Net/Net.o ${LIB_SYSTEM_DIR}/Net/Socket/Socket.o ${LIB_SYSTEM_DIR}/Net/Protocol/Protocol.o \
	${LIB_SYSTEM_DIR}/Net/Protocol/HTTP/HTTP.o \
	${LIB_SYSTEM_DIR}/Crypt/Crypt.o ${LIB_SYSTEM_DIR}/Crypt/SHA1/SHA1.o

LIB_SYSTEM_CFLAGS  = ${CFLAGS}
LIB_SYSTEM_LDFLAGS = ${LDFLAGS}

all: ljs libcore libsystem

ljs:
	${CXX} ${CORE_LDFLAGS} ${CORE_CFLAGS} ${CORE} -o ljs

core_install:
	mkdir -p ${LJS_LIBDIR}
	cp -f ljs ${BINDIR}/

libcore: $(LIB_CORE)

$(LIB_CORE): $(LIB_CORE:.o=.cpp)
	${CXX} ${LIB_CORE_CFLAGS} -fPIC -c $*.cpp -o $*.lo
	${CXX} ${LIB_CORE_LDFLAGS} -dynamiclib -shared -o $*.o $*.lo -lc

libcore_install: libcore
	mkdir -p ${LJS_LIBDIR}
	mkdir -p ${LJS_LIBDIR}/Core
	mkdir -p ${LJS_LIBDIR}/Core/Thread
########
	cp -f  ${LIB_CORE_DIR}/init.js				${LJS_LIBDIR}/Core/init.js
########
	cp -rf ${LIB_CORE_DIR}/Object.o				${LJS_LIBDIR}/Core/Object.so
########
	cp -f  ${LIB_CORE_DIR}/Thread/init.js		${LJS_LIBDIR}/Core/Thread/init.js
	cp -f  ${LIB_CORE_DIR}/Thread/Thread.o		${LJS_LIBDIR}/Core/Thread/Thread.so
########
	cp -rf ${LIB_CORE_DIR}/*.js					${LJS_LIBDIR}/Core/
	
libsystem: $(LIB_SYSTEM)

$(LIB_SYSTEM): $(LIB_SYSTEM:.o=.cpp)
	${CXX} ${LIB_SYSTEM_CFLAGS} -fPIC -c $*.cpp -o $*.lo
	${CXX} ${LIB_SYSTEM_LDFLAGS} -dynamiclib -shared -o $*.o $*.lo -lc

libsystem_install: libsystem
	mkdir -p ${LJS_LIBDIR}
	mkdir -p ${LJS_LIBDIR}/System
	mkdir -p ${LJS_LIBDIR}/System/Environment
	mkdir -p ${LJS_LIBDIR}/System/Console
	mkdir -p ${LJS_LIBDIR}/System/FileSystem
	mkdir -p ${LJS_LIBDIR}/System/FileSystem/File
	mkdir -p ${LJS_LIBDIR}/System/FileSystem/Directory
	mkdir -p ${LJS_LIBDIR}/System/FileSystem/Permission
	mkdir -p ${LJS_LIBDIR}/System/FileSystem/Time
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
	cp -f ${LIB_SYSTEM_DIR}/Environment/init.js                             ${LJS_LIBDIR}/System/Environment/init.js
	cp -f ${LIB_SYSTEM_DIR}/Environment/Environment.o			${LJS_LIBDIR}/System/Environment/Environment.so
########
	cp -f ${LIB_SYSTEM_DIR}/FileSystem/init.js					${LJS_LIBDIR}/System/FileSystem/init.js
	cp -f ${LIB_SYSTEM_DIR}/FileSystem/FileSystem.o				${LJS_LIBDIR}/System/FileSystem/FileSystem.so
	cp -f ${LIB_SYSTEM_DIR}/FileSystem/FileSystem.js			${LJS_LIBDIR}/System/FileSystem/FileSystem.js
########
	cp -f ${LIB_SYSTEM_DIR}/FileSystem/File/init.js				${LJS_LIBDIR}/System/FileSystem/File/init.js
	cp -f ${LIB_SYSTEM_DIR}/FileSystem/File/File.o				${LJS_LIBDIR}/System/FileSystem/File/File.so
	cp -f ${LIB_SYSTEM_DIR}/FileSystem/File/File.js				${LJS_LIBDIR}/System/FileSystem/File/File.js
########
	cp -f ${LIB_SYSTEM_DIR}/FileSystem/Directory/init.js		${LJS_LIBDIR}/System/FileSystem/Directory/init.js
	cp -f ${LIB_SYSTEM_DIR}/FileSystem/Directory/Directory.o	${LJS_LIBDIR}/System/FileSystem/Directory/Directory.so
	cp -f ${LIB_SYSTEM_DIR}/FileSystem/Directory/Directory.js	${LJS_LIBDIR}/System/FileSystem/Directory/Directory.js
########
	cp -f ${LIB_SYSTEM_DIR}/FileSystem/Permission/init.js		${LJS_LIBDIR}/System/FileSystem/Permission/init.js
	cp -f ${LIB_SYSTEM_DIR}/FileSystem/Permission/Permission.js	${LJS_LIBDIR}/System/FileSystem/Permission/Permission.js
	cp -f ${LIB_SYSTEM_DIR}/FileSystem/Permission/Mode.js		${LJS_LIBDIR}/System/FileSystem/Permission/Mode.js
########
	cp -f ${LIB_SYSTEM_DIR}/FileSystem/Time/init.js				${LJS_LIBDIR}/System/FileSystem/Time/init.js
	cp -f ${LIB_SYSTEM_DIR}/FileSystem/Time/Time.js				${LJS_LIBDIR}/System/FileSystem/Time/Time.js
########
	cp -f ${LIB_SYSTEM_DIR}/Console/init.js						${LJS_LIBDIR}/System/Console/init.js
	cp -f ${LIB_SYSTEM_DIR}/Console/Console.o					${LJS_LIBDIR}/System/Console/Console.so
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

