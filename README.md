# leviathanK
leviathanK is a project for browsing a Windows registry-like path tree data structure. 
In its foundation there is a tree-like data structure, each node of the tree is identified by a path consisting of a hierarchical sequence of parent node names, ending with the node's own name, separated by a delimiter ('/'). 
Example of one such tree:
```
[ROOT]: [EMPTY] [0x7f61422a9130]
one: [EMPTY] [0x7f61422a9164]
one/two: [EMPTY] [0x7f61422a91a8]
one/two/three: [EMPTY] [0x7f61422a91ec]
one/two/three/four: 4 [0x7f61422a9232]
```
The application has a very basic shell that has the following commands:
- `set` command creates new nodes and sets values for existing ones;
- `print` command prints the current state of the tree or its parts;
- `exit` command doesn't take any arguments and just exits the application;
- `test` command serves only the purpose of testing: with no arguments it says "No arguments provided", with a single argument it says "Argument: [the argument you provided]".

## set command
- `set` - no arguments set is considered invalid because path to the new node cannot be an empty string.
- `set <path>` - either creates a new node at \<path\> and sets its value to NULL if the node does not exist yet, or empties the value at \<path\> if it had already existed before the command call.
- `set <path> <value>` - sets node at \<path\> to the \<value\>.
  - `<path>` - a `/`- separated sequence of node names. For example, if the path you have specified is `one/two/three`, the command creates three nodes: node `one`, node `two` as `one`'s child, and node `three` as `two`'s child, so the chain of nodes becomes `one` -> `two` -> `three`. If there's no `/` in the specified path, the command creates just one node with that name.
  **NOTE** the \<path\> argument does not support spaces currently. So even though the data structure itself supports spaces, the shell does not. So it's impossible to create a path such as `this/is/a spaced/path` with the current version of the shell.
  - `<value>` is a value to set for the node at \<path\>. `<value>` is everything after \<path\>\<spaces\>. \<value\> starts at a first non-space character after \<path\> and ends with the newline character, that is, at the end of the command string. That's why a command string like `set this/is/a/node several words in a value` is perfectly valid and will set the string `several words in a value` as a value for the node `node` at the end of the `this` -> `is` -> `a` -> `node` chain. If the entire chain did not exist, new nodes `this`, `is` and `a` will be created empty (with NULL values), while the node `node` will be created with the specified `several words in a value` string as its value. If \<value\> is not specified and the set command call only consists of `set <path>`, then the node at \<path\> will be created with an empty (NULL) value. If the value of the node at \<path\> had already existed before the `set <path>` command call, this value will be emptied (set to NULL).

## print command
- `print` - prints the entire tree non-verbosely. "Non-verbosely" means that the printing omits empty nodes and only prints `<node name>: <node value>` for each node.
- `print verbose` - prints the entire tree verbosely. "Verbosely" means that it prints **all** the nodes including the empty ones, and the format is `<node name>: <node value> [<hex pointer to the node>]`
- `print <path>` - prints node at \<path\> and its children if present non-verbosely. In this case, it prints the node itself even if it is empty, but none of its empty children would get printed.
- `print verbose <path>` - prints node at \<path\> and all its children if present verbosely. The format is the same as for `print verbose` call that prints the entire tree out.

### print examples
Let's assume we have the following tree:
```
[ROOT]: [EMPTY] [0x7f384fdae130]
a: [EMPTY] [0x7f384fdae157]
a/b: [EMPTY] [0x7f384fdae199]
a/b/c: [EMPTY] [0x7f384fdae1db]
a/b/c/d: [EMPTY] [0x7f384fdae21d]
i: [EMPTY] [0x7f384fdae27c]
i/am: [EMPTY] [0x7f384fdae2c3]
i/am/value: whoops! [0x7f384fdae306]
i/am/value/filled: [EMPTY] [0x7f384fdae34c]
i/am/value/filled/node: value [0x7f384fdae393]
```
Note how the `a/b/c/d` node chain is completely empty. The `i/am/value/filled/node` chain has two nodes with non-empty values: `i/am/value` has the value of `whoops!` and `i/am/value/filled/node` has the value of `value`. 

In this case, `print` call would print:
```
i/am/value: whoops!
i/am/value/filled/node: value
```
Note how the non-verbose `print` call omits all the empty nodes.

The `print verbose` would print the output that is shown at the beginning of this section, with a bunch of `[EMPTY]`'s and hex values of node pointers.

The `print a/b/c/d` would print:
`Requested node -> a/b/c/d: [EMPTY]`
The reason for that is that it is an endpoint node (doesn't have any children) and is empty itself.

The `print i/am/value` would print:
```
Requested node -> i/am/value: whoops!
Node's children:
filled/node: value
```

The `print verbose a/b/c/d` would print:
`d: [EMPTY] [0x7f384fdae21d]`

The `print verbose i/am/value` would print:
```
value: whoops! [0x7f384fdae306]
filled: [EMPTY] [0x7f384fdae34c]
filled/node: value [0x7f384fdae393]
```

**NOTE**: this print implementation has a couple of bugs that haven't been fixed. For comprehensive details on the bugs please refer to issue [#27](https://github.com/Oleksii-Kshenskyi/leviathanK/issues/27).

# How to build?

At the moment, this application is only buildable on Linux. It uses a libc getline() implementation from the `#include <stdio.h>` header, and that is unfortunately unavailable on Windows. To build this on Windows, one would have to implement getline() themself first.

On Linux, the application requires:
- cmake (version >= 3.10);
- ninja build system (version used -> 1.10.0);
- Unix Makefiles (version used -> 4.3);
- A compiler that supports the C11 standard (CMake uses GCC by default, version used is 9.3.0-1);
- A debugger such as gdb for debugging. Debugging is already enabled in CMake on the master branch;
- valgrind if you want to test the validity of application's memory usage.

## On Manjaro/Arch
- `sudo pacman -S cmake`
- `sudo pacman -S ninja`
- `make rebuild` <- rebuilds the application from scratch
- `make` or `make run` <- builds the application and runs it
- `make dbg` <- runs gdb with the application for debugging
- `make justrun` <- does not compile anything, just runs the executable. Will fail if the application has not been compiled yet.
- `make valgrind` <- runs the application under valgrind if that is installed. (installing valgrind is `sudo pacman -S valgrind` as one would expect).

## On CentOS 7
- `sudo yum install cmake3`
- `sudo yum install ninja-build`
- change cmake call in the Makefile from `cd build && cmake .. -G "Ninja"` to `cd build && cmake3 .. -G "Ninja"` because on CentOS 7, cmake 3 is called cmake3 instead of just cmake.
- change ninja call in the Make file from `cd build && ninja` to `cd build && ninja-build` because on CentOS 7, the ninja build system is called ninja-build instead of just ninja.
- `sudo yum install valgrind` for valgrind.
- same make commands as for Manjaro/Arch to control the application execution and building.

## On any other Linux
Use your system's package manager to install:
- cmake 3
- ninja build system
- valgrind if needed

Then make sure that the executables for those are called `cmake`, `ninja` and `valgrind` on your system. If not, change the executable calls in the Makefile accordingly.

# The 2020 Challenge
This project is a part of the 2020 challenge where I would start and finish one IT (mostly programming except for one month) project within one month, one project a month starting from Feb 2020. This project is the April 2020 project (Run One, which is currently the only one for this project, started on 04/01/2020 and ended on 04/30/2020). This is the reason why there were some bugs left in the print implementation and features like deleting a node and saving trees to/restoring from a file were not implemented, due to the 04/30/2020 deadline. For precise statistics on estimations and time spent on this project, please refer to issue [#1](https://github.com/Oleksii-Kshenskyi/leviathanK/issues/1) of this project.