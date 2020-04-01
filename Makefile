.SILENT: all clean run

all:
	mkdir build
	cd build && cmake ..
	cd build && make

clean: build
	rm -rf build

run: build/leviathanK
	build/leviathanK