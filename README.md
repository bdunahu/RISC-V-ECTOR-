# RISC V[ECTOR]

## Dependencies

- cmake
- g++ (GCC) 11.4.0
- catch2 version 3.5.3
- Qt version 6.8.2
- RAM (a custom memory submodule)

## to compile
Generate the build directory with

`cmake -S . -B build`

then compile both the simulator and tests with

`cmake --build build`

To develop, the following git option is useful to keep modules updated.:

`git config submodule.recurse true`

# about

Created at the University of Massachusetts, Amherst

CS535 -- Computer Architecture and ISA Design
