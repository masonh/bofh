CXX ?= g++
CC ?= g++
LD ?= g++
CXXFLAGS := ${CXXFLAGS} ${CCFLAGS}
CXXFLAGS += --std=c++0x -s -O2
PREFIX ?= /usr/local
BINDIR ?= ${PREFIX}/bin
DATADIR ?= ${PREFIX}/share
MANDIR ?= ${DATADIR}/man
DOCDIR ?= ${DATADIR}/doc

all: linux linux32 windows windows32
all64: windows linux
all32: windows32 linux32
linux: bofh
linux32: bofh32
linux64: linux
windows: bofh.exe
windows32: bofh32.exe
windows64: windows

bofh: bofh.cpp
	${CXX} ${CPPFLAGS} ${CXXFLAGS} -o $@ bofh.cpp
bofh32: bofh.cpp
	${CXX} ${CPPFLAGS} -m32 ${CXXFLAGS} -o $@ bofh.cpp
bofh.exe: bofh.cpp
	x86_64-w64-mingw32-g++ ${CPPFLAGS} ${CXXFLAGS} -static -DWIN32 -o $@ bofh.cpp
bofh32.exe: bofh.cpp
	i686-w64-mingw32-g++ ${CPPFLAGS} ${CXXFLAGS} -static -DWIN32 -o $@ bofh.cpp
clean:
	rm -f bofh bofh.exe bofh32 bofh32.exe
install:
	@cp -p bofh ${DESTDIR}${BINDIR}/bofh
	@cp -p bofh32 ${DESTDIR}${BINDIR}/bofh32
	@cp -p README ${DESTDIR}${DOCDIR}/bofh/README
	@cp -p instructions.txt ${DESTDIR}${DOCDIR}/bofh/instructions.txt

.PHONY: clean install all all32 all64 linux linux32 linux64 windows windows32 windows64
