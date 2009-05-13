VERSION = 0.2.0

CC         = gcc
CXX        = g++
BINDIR     = /usr/bin
LJS_LIBDIR = /usr/lib/lulzjs
CFLAGS     = -D__LJS_LIBRARY_PATH__="\"${LJS_LIBDIR}\"" -D__LJS_VERSION__="\"${VERSION}\"" $(shell lulzjs-config --cflags) -I./src/core -I./src/lib 
LDFLAGS    = $(shell lulzjs-config --libs | sed 's/-dynamic.*/-lm/')

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
CORE         = ${CORE_DIR}/main.o ${CORE_DIR}/Interactive.o
CORE_CFLAGS  = ${CFLAGS}
CORE_LDFLAGS = ${LDFLAGS} -lreadline -lncurses

## LIB_CORE ##
LIB_CORE_DIR = src/core/Core
LIB_CORE = \
	${LIB_CORE_DIR}/Core.o ${LIB_CORE_DIR}/Object.o ${LIB_CORE_DIR}/init.o

LIB_CORE_CFLAGS  = ${CFLAGS}
LIB_CORE_LDFLAGS = ${LDFLAGS}

## LIB_SYSTEM ##

LIB_SYSTEM_DIR = src/lib/System
LIB_SYSTEM = \
	${LIB_SYSTEM_DIR}/System.o \
	${LIB_SYSTEM_DIR}/Thread/Thread.o \
	${LIB_SYSTEM_DIR}/Environment/Environment.o \
	${LIB_SYSTEM_DIR}/Console/Console.o \
	${LIB_SYSTEM_DIR}/FileSystem/FileSystem.o ${LIB_SYSTEM_DIR}/FileSystem/File/File.o ${LIB_SYSTEM_DIR}/FileSystem/Directory/Directory.o \
	${LIB_SYSTEM_DIR}/Network/Network.o \
	${LIB_SYSTEM_DIR}/Network/Sockets/Sockets.o ${LIB_SYSTEM_DIR}/Network/Sockets/TCP.o ${LIB_SYSTEM_DIR}/Network/Sockets/UDP.o ${LIB_SYSTEM_DIR}/Network/Sockets/ICMP.o ${LIB_SYSTEM_DIR}/Network/Sockets/RAW.o \
	${LIB_SYSTEM_DIR}/Network/Protocol/Protocol.o ${LIB_SYSTEM_DIR}/Network/Protocol/HTTP/HTTP.o ${LIB_SYSTEM_DIR}/Network/Protocol/IRC/IRC.o \

LIB_SYSTEM_CFLAGS  = ${CFLAGS}
LIB_SYSTEM_LDFLAGS = ${LDFLAGS}

## LIB_MATH ##

LIB_MATH_DIR = src/lib/Math
LIB_MATH = \
	${LIB_MATH_DIR}/Math.o \
	${LIB_MATH_DIR}/Cryptography/Cryptography.o \
	${LIB_MATH_DIR}/Cryptography/Hashing/Hashing.o ${LIB_MATH_DIR}/Cryptography/Hashing/SHA1/SHA1.o \
	${LIB_MATH_DIR}/Cryptography/Crypting/Crypting.o \
	${LIB_MATH_DIR}/Sequences/Sequences.o 

LIB_MATH_CFLAGS  = ${CFLAGS}
LIB_MATH_LDFLAGS = ${LDFLAGS}

all: ljs libcore libsystem libmath

ljs: $(CORE)
	${CXX} ${CORE_LDFLAGS} ${CORE_CFLAGS} $(CORE:.o=.cpp) -o ljs

$(CORE): $(CORE:.o=.cpp)
	${CXX} ${CORE_CFLAGS} -c $*.cpp -o $*.o

core_install:
	mkdir -p ${LJS_LIBDIR}
	cp -f ljs ${BINDIR}/
	cp -f src/core/ljs_arguments.js ${LJS_LIBDIR}

libcore: $(LIB_CORE)

$(LIB_CORE): $(LIB_CORE:.o=.cpp)
	${CXX} ${LIB_CORE_CFLAGS} -fPIC -c $*.cpp -o $*.lo
	${CXX} ${LIB_CORE_LDFLAGS} -dynamiclib -shared -o $*.o $*.lo -lc

libcore_install: libcore
	mkdir -p ${LJS_LIBDIR}
	mkdir -p ${LJS_LIBDIR}/Core
########
	cp -f  ${LIB_CORE_DIR}/init.js				${LJS_LIBDIR}/Core/init.js
	cp -f  ${LIB_CORE_DIR}/init.o				${LJS_LIBDIR}/Core/init.so
	cp -f  ${LIB_CORE_DIR}/Core.o				${LJS_LIBDIR}/Core/Core.so
########
	cp -rf ${LIB_CORE_DIR}/Object.o				${LJS_LIBDIR}/Core/Object.so
########
	cp -rf ${LIB_CORE_DIR}/*.js					${LJS_LIBDIR}/Core/
	
libsystem: $(LIB_SYSTEM)

$(LIB_SYSTEM): $(LIB_SYSTEM:.o=.cpp)
	${CXX} ${LIB_SYSTEM_CFLAGS} -fPIC -c $*.cpp -o $*.lo
	${CXX} ${LIB_SYSTEM_LDFLAGS} -dynamiclib -shared -o $*.o $*.lo -lc

libsystem_install: libsystem
	mkdir -p ${LJS_LIBDIR}
	mkdir -p ${LJS_LIBDIR}/System
	mkdir -p ${LJS_LIBDIR}/System/Thread
	mkdir -p ${LJS_LIBDIR}/System/Environment
	mkdir -p ${LJS_LIBDIR}/System/Console
	mkdir -p ${LJS_LIBDIR}/System/FileSystem
	mkdir -p ${LJS_LIBDIR}/System/FileSystem/File
	mkdir -p ${LJS_LIBDIR}/System/FileSystem/Directory
	mkdir -p ${LJS_LIBDIR}/System/FileSystem/Permission
	mkdir -p ${LJS_LIBDIR}/System/FileSystem/Time
	mkdir -p ${LJS_LIBDIR}/System/Network
	mkdir -p ${LJS_LIBDIR}/System/Network/Sockets
	mkdir -p ${LJS_LIBDIR}/System/Network/Ports
	mkdir -p ${LJS_LIBDIR}/System/Network/Protocol
	mkdir -p ${LJS_LIBDIR}/System/Network/Protocol/HTTP
	mkdir -p ${LJS_LIBDIR}/System/Network/Protocol/HTTP/Simple
	mkdir -p ${LJS_LIBDIR}/System/Network/Protocol/IRC
	mkdir -p ${LJS_LIBDIR}/System/Crypt
	mkdir -p ${LJS_LIBDIR}/System/Crypt/SHA1
########
	cp -f ${LIB_SYSTEM_DIR}/init.js										${LJS_LIBDIR}/System/init.js
	cp -f ${LIB_SYSTEM_DIR}/System.o									${LJS_LIBDIR}/System/System.so
########
	cp -f ${LIB_SYSTEM_DIR}/Thread/init.js								${LJS_LIBDIR}/System/Thread/init.js
	cp -f ${LIB_SYSTEM_DIR}/Thread/Thread.o								${LJS_LIBDIR}/System/Thread/Thread.so
########
	cp -f ${LIB_SYSTEM_DIR}/Environment/init.js							${LJS_LIBDIR}/System/Environment/init.js
	cp -f ${LIB_SYSTEM_DIR}/Environment/Environment.o					${LJS_LIBDIR}/System/Environment/Environment.so
########
	cp -f ${LIB_SYSTEM_DIR}/FileSystem/init.js							${LJS_LIBDIR}/System/FileSystem/init.js
	cp -f ${LIB_SYSTEM_DIR}/FileSystem/FileSystem.o						${LJS_LIBDIR}/System/FileSystem/FileSystem.so
	cp -f ${LIB_SYSTEM_DIR}/FileSystem/FileSystem.js					${LJS_LIBDIR}/System/FileSystem/FileSystem.js
########
	cp -f ${LIB_SYSTEM_DIR}/FileSystem/File/init.js						${LJS_LIBDIR}/System/FileSystem/File/init.js
	cp -f ${LIB_SYSTEM_DIR}/FileSystem/File/File.o						${LJS_LIBDIR}/System/FileSystem/File/File.so
	cp -f ${LIB_SYSTEM_DIR}/FileSystem/File/File.js						${LJS_LIBDIR}/System/FileSystem/File/File.js
########
	cp -f ${LIB_SYSTEM_DIR}/FileSystem/Directory/init.js				${LJS_LIBDIR}/System/FileSystem/Directory/init.js
	cp -f ${LIB_SYSTEM_DIR}/FileSystem/Directory/Directory.o			${LJS_LIBDIR}/System/FileSystem/Directory/Directory.so
	cp -f ${LIB_SYSTEM_DIR}/FileSystem/Directory/Directory.js			${LJS_LIBDIR}/System/FileSystem/Directory/Directory.js
########
	cp -f ${LIB_SYSTEM_DIR}/FileSystem/Permission/init.js				${LJS_LIBDIR}/System/FileSystem/Permission/init.js
	cp -f ${LIB_SYSTEM_DIR}/FileSystem/Permission/Permission.js			${LJS_LIBDIR}/System/FileSystem/Permission/Permission.js
	cp -f ${LIB_SYSTEM_DIR}/FileSystem/Permission/Mode.js				${LJS_LIBDIR}/System/FileSystem/Permission/Mode.js
########
	cp -f ${LIB_SYSTEM_DIR}/FileSystem/Time/init.js						${LJS_LIBDIR}/System/FileSystem/Time/init.js
	cp -f ${LIB_SYSTEM_DIR}/FileSystem/Time/Time.js						${LJS_LIBDIR}/System/FileSystem/Time/Time.js
########
	cp -f ${LIB_SYSTEM_DIR}/Console/init.js								${LJS_LIBDIR}/System/Console/init.js
	cp -f ${LIB_SYSTEM_DIR}/Console/Console.o							${LJS_LIBDIR}/System/Console/Console.so
	cp -f ${LIB_SYSTEM_DIR}/Console/Console.js							${LJS_LIBDIR}/System/Console/Console.js
#######
	cp -f ${LIB_SYSTEM_DIR}/Network/init.js								${LJS_LIBDIR}/System/Network/init.js
	cp -f ${LIB_SYSTEM_DIR}/Network/Network.o							${LJS_LIBDIR}/System/Network/Network.so
#######
	cp -f ${LIB_SYSTEM_DIR}/Network/Sockets/init.js						${LJS_LIBDIR}/System/Network/Sockets/init.js
	cp -f ${LIB_SYSTEM_DIR}/Network/Sockets/Sockets.o					${LJS_LIBDIR}/System/Network/Sockets/Sockets.so
	cp -f ${LIB_SYSTEM_DIR}/Network/Sockets/Sockets.js					${LJS_LIBDIR}/System/Network/Sockets/Sockets.js
	cp -f ${LIB_SYSTEM_DIR}/Network/Sockets/TCP.o						${LJS_LIBDIR}/System/Network/Sockets/TCP.so
	cp -f ${LIB_SYSTEM_DIR}/Network/Sockets/TCP.js						${LJS_LIBDIR}/System/Network/Sockets/TCP.js
	cp -f ${LIB_SYSTEM_DIR}/Network/Sockets/UDP.o						${LJS_LIBDIR}/System/Network/Sockets/UDP.so
	cp -f ${LIB_SYSTEM_DIR}/Network/Sockets/UDP.js						${LJS_LIBDIR}/System/Network/Sockets/UDP.js
	cp -f ${LIB_SYSTEM_DIR}/Network/Sockets/ICMP.o						${LJS_LIBDIR}/System/Network/Sockets/ICMP.so
	cp -f ${LIB_SYSTEM_DIR}/Network/Sockets/ICMP.js						${LJS_LIBDIR}/System/Network/Sockets/ICMP.js
	cp -f ${LIB_SYSTEM_DIR}/Network/Sockets/RAW.o						${LJS_LIBDIR}/System/Network/Sockets/RAW.so
	cp -f ${LIB_SYSTEM_DIR}/Network/Sockets/RAW.js						${LJS_LIBDIR}/System/Network/Sockets/RAW.js
#######
	cp -f ${LIB_SYSTEM_DIR}/Network/Ports/init.js						${LJS_LIBDIR}/System/Network/Ports/init.js
	cp -f ${LIB_SYSTEM_DIR}/Network/Ports/Ports.js						${LJS_LIBDIR}/System/Network/Ports/Ports.js
#######
	cp -f ${LIB_SYSTEM_DIR}/Network/Protocol/init.js					${LJS_LIBDIR}/System/Network/Protocol/init.js
	cp -f ${LIB_SYSTEM_DIR}/Network/Protocol/Protocol.o					${LJS_LIBDIR}/System/Network/Protocol/Protocol.so
#######
	cp -f ${LIB_SYSTEM_DIR}/Network/Protocol/HTTP/init.js				${LJS_LIBDIR}/System/Network/Protocol/HTTP/init.js
	cp -f ${LIB_SYSTEM_DIR}/Network/Protocol/HTTP/HTTP.o				${LJS_LIBDIR}/System/Network/Protocol/HTTP/HTTP.so
	cp -f ${LIB_SYSTEM_DIR}/Network/Protocol/HTTP/HTTP.js				${LJS_LIBDIR}/System/Network/Protocol/HTTP/HTTP.js
	cp -f ${LIB_SYSTEM_DIR}/Network/Protocol/HTTP/Request.js 			${LJS_LIBDIR}/System/Network/Protocol/HTTP/Request.js
	cp -f ${LIB_SYSTEM_DIR}/Network/Protocol/HTTP/Response.js			${LJS_LIBDIR}/System/Network/Protocol/HTTP/Response.js
	cp -f ${LIB_SYSTEM_DIR}/Network/Protocol/HTTP/Client.js				${LJS_LIBDIR}/System/Network/Protocol/HTTP/Client.js
#######
	cp -f ${LIB_SYSTEM_DIR}/Network/Protocol/HTTP/Simple/init.js		${LJS_LIBDIR}/System/Network/Protocol/HTTP/Simple/init.js
	cp -f ${LIB_SYSTEM_DIR}/Network/Protocol/HTTP/Simple/Simple.js		${LJS_LIBDIR}/System/Network/Protocol/HTTP/Simple/Simple.js
#######
	cp -f ${LIB_SYSTEM_DIR}/Network/Protocol/IRC/init.js				${LJS_LIBDIR}/System/Network/Protocol/IRC/init.js
	cp -f ${LIB_SYSTEM_DIR}/Network/Protocol/IRC/IRC.o					${LJS_LIBDIR}/System/Network/Protocol/IRC/IRC.so
	cp -f ${LIB_SYSTEM_DIR}/Network/Protocol/IRC/IRC.js					${LJS_LIBDIR}/System/Network/Protocol/IRC/IRC.js
	cp -f ${LIB_SYSTEM_DIR}/Network/Protocol/IRC/Client.js				${LJS_LIBDIR}/System/Network/Protocol/IRC/Client.js

libsystem_uninstall:

libmath: $(LIB_MATH)

$(LIB_MATH): $(LIB_MATH:.o=.cpp)
	${CXX} ${LIB_MATH_CFLAGS} -fPIC -c $*.cpp -o $*.lo
	${CXX} ${LIB_MATH_LDFLAGS} -dynamiclib -shared -o $*.o $*.lo -lc

libmath_install: libmath
	mkdir -p ${LJS_LIBDIR}
	mkdir -p ${LJS_LIBDIR}/Math
	mkdir -p ${LJS_LIBDIR}/Math/Cryptography
	mkdir -p ${LJS_LIBDIR}/Math/Cryptography/Hashing
	mkdir -p ${LJS_LIBDIR}/Math/Cryptography/Hashing/SHA1
	mkdir -p ${LJS_LIBDIR}/Math/Cryptography/Crypting
	mkdir -p ${LJS_LIBDIR}/Math/Sequences/
	mkdir -p ${LJS_LIBDIR}/Math/Sequences/Fibonacci
########
	cp -f ${LIB_MATH_DIR}/init.js									${LJS_LIBDIR}/Math/init.js
	cp -f ${LIB_MATH_DIR}/Math.o									${LJS_LIBDIR}/Math/Math.so
########
	cp -f ${LIB_MATH_DIR}/Cryptography/init.js						${LJS_LIBDIR}/Math/Cryptography/init.js
	cp -f ${LIB_MATH_DIR}/Cryptography/Cryptography.o				${LJS_LIBDIR}/Math/Cryptography/Cryptography.so
	cp -f ${LIB_MATH_DIR}/Cryptography/Cryptography.js				${LJS_LIBDIR}/Math/Cryptography/Cryptography.js
########
	cp -f ${LIB_MATH_DIR}/Cryptography/Hashing/init.js				${LJS_LIBDIR}/Math/Cryptography/Hashing/init.js
	cp -f ${LIB_MATH_DIR}/Cryptography/Hashing/Hashing.o			${LJS_LIBDIR}/Math/Cryptography/Hashing/Hashing.so
	cp -f ${LIB_MATH_DIR}/Cryptography/Hashing/Hashing.js			${LJS_LIBDIR}/Math/Cryptography/Hashing/Hashing.js
########
	cp -f ${LIB_MATH_DIR}/Cryptography/Hashing/SHA1/init.js			${LJS_LIBDIR}/Math/Cryptography/Hashing/SHA1/init.js
	cp -f ${LIB_MATH_DIR}/Cryptography/Hashing/SHA1/SHA1.o			${LJS_LIBDIR}/Math/Cryptography/Hashing/SHA1/SHA1.so
	cp -f ${LIB_MATH_DIR}/Cryptography/Hashing/SHA1/SHA1.js			${LJS_LIBDIR}/Math/Cryptography/Hashing/SHA1/SHA1.js
########
	cp -f ${LIB_MATH_DIR}/Cryptography/Crypting/init.js				${LJS_LIBDIR}/Math/Cryptography/Crypting/init.js
	cp -f ${LIB_MATH_DIR}/Cryptography/Crypting/Crypting.o			${LJS_LIBDIR}/Math/Cryptography/Crypting/Crypting.so
	cp -f ${LIB_MATH_DIR}/Cryptography/Crypting/Crypting.js			${LJS_LIBDIR}/Math/Cryptography/Crypting/Crypting.js
########
	cp -f ${LIB_MATH_DIR}/Sequences/init.js							${LJS_LIBDIR}/Math/Sequences/init.js
	cp -f ${LIB_MATH_DIR}/Sequences/Sequences.o						${LJS_LIBDIR}/Math/Sequences/Sequences.so
	cp -f ${LIB_MATH_DIR}/Sequences/Sequences.js					${LJS_LIBDIR}/Math/Sequences/Sequences.js
########
	cp -f ${LIB_MATH_DIR}/Sequences/Fibonacci/init.js				${LJS_LIBDIR}/Math/Sequences/Fibonacci/init.js
	cp -f ${LIB_MATH_DIR}/Sequences/Fibonacci/Fibonacci.js			${LJS_LIBDIR}/Math/Sequences/Fibonacci/Fibonacci.js


libmath_uninstall:


install: all core_install libcore_install libsystem_install libmath_install
	chmod -R a+rx ${LJS_LIBDIR}
	chmod a+rx ${BINDIR}/ljs

uninstall:
	rm -f ${BINDIR}/ljs
	rm -f  ${LJS_LIBDIR}/ljs_arguments.js
	rm -rf ${LJS_LIBDIR}/Core
	rm -rf ${LJS_LIBDIR}/System
	rm -rf ${LJS_LIBDIR}/Math
	

clean:
	rm -f ljs;
	find src|egrep "\.l?o"|xargs rm -f

