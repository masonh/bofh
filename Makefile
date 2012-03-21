all: windows windows32 linux linux32
all64: windows linux
all32: windows32 linux32
linux: bofh
linux32: bofh32
linux64: linux
windows: bofh.exe
windows32: bofh32.exe
windows64: windows

bofh: bofh.cpp
	g++ --std=c++0x -o $@ bofh.cpp
bofh32: bofh.cpp
	g++ -m32 --std=c++0x -o $@ bofh.cpp
bofh32.exe: bofh.cpp
	i686-w64-mingw32-g++ --std=c++0x -static -DWIN32 -o $@ bofh.cpp
bofh.exe: bofh.cpp
	x86_64-w64-mingw32-g++ --std=c++0x -static -DWIN32 -o $@ bofh.cpp
clean:
	rm -f bofh bofh.exe bofh32 bofh32.exe
.PHONY: clean linux linux32 linux64 windows windows32 windows64 all all32 all64

