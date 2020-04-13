.SILENT: all clean run compile justrun rebuild win

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

win: clean compile
	build/leviathanK.exe