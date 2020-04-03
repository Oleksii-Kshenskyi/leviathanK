.SILENT: all clean run compile justrun rebuild

all: compile

compile:
	mkdir -p build
	cd build && cmake ..
	cd build && make

clean:
	rm -rf build

rebuild: clean compile

run: compile
	build/leviathanK

justrun: build/leviathanK
	build/leviathanK