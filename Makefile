bofh: bofh.cpp
	g++ -o bofh --std=c++0x bofh.cpp
clean:
	rm -f bofh
.PHONY: clean

