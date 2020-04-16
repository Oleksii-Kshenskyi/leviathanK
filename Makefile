.SILENT: all clean run compile justrun rebuild win valgrind

all: rebuild justrun

compile:
	mkdir -p build
	cd build && cmake .. -G "Ninja"
	cd build && ninja

clean:
	rm -rf build

rebuild: clean compile

run: compile
	build/leviathanK

justrun: build/leviathanK
	build/leviathanK

valgrind: compile
	valgrind -s --leak-check=full --show-leak-kinds=all build/leviathanK

win: clean compile
	build/leviathanK.exe