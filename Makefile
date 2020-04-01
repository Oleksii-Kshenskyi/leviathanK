.SILENT: all clean run

all:
	mkdir -p build
	cd build && cmake ..
	cd build && make

clean:
	rm -rf build

run: all
	build/leviathanK