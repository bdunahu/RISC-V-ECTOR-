# RISC V[ECTOR]

![RISC V[ECTOR] UI computing a matrix multiply](./assets/risc_vector.png)

## Dependencies

This is a ISA simulator for a mini-ISA including vector support.

- cmake
- g++ (GCC) 11.4.0
- catch2 version 3.5.3
- Qt version 6.8.2
- [RAM](https://github.com/bdunahu/ram) (a custom memory submodule)

Additionally, see [rva](https://github.com/bdunahu/rva) for a custom assembler which generates a compatible binary.

## to compile
Generate the build directory with

`cmake -S . -B build`

then compile both the simulator and tests with

`cmake --build build`

To develop, the following git option is useful to keep modules updated:

`git config submodule.recurse true`

# about

Created at the University of Massachusetts, Amherst

CS535 -- Computer Architecture and ISA Design
