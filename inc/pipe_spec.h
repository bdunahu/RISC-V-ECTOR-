// Simulator for the RISC-V[ECTOR] mini-ISA
// Copyright (C) 2025 Siddarth Suresh
// Copyright (C) 2025 bdunahu

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#ifndef PIPE_SPEC_H
#define PIPE_SPEC_H
#include "logger.h"
#include <cmath>

/**
 * The number of bits to specify a word in a line
 */
#define LINE_SPEC 2
/**
 * The total number of words in a line
 */
#define LINE_SIZE static_cast<int>(pow(2, 2))

/**
 * The number of general purpose registers
 */
#define GPR_NUM 16

/**
 * The number of vector registers
 */
#define V_NUM 8

/**
 * The size, in 32-bit integers, of a vector register.
 */
#define V_R_LIMIT 8

/**
 * The number of bits to specify an instruction type
 */
#define TYPE_SIZE 2

/**
 * The number of bits to specify a register
 */
#define REG_SIZE 5

/**
 * The number of bits to specify an R-Type opcode.
 */
#define R_OPCODE_SIZE 5

/**
 * The number of bits to specify an opcode.
 */
#define OPCODE_SIZE 4

/**
 * The maximum value an integer can hold.
 * The minimum is always this number plus one negated.
 */
#define MAX_INT 2147483647

/**
 * The delay on DRAM objects.
 */
#define DRAM_DELAY 10

/**
 * The (base) on cache objects.
 */
#define CACHE_DELAY 1

#define VECTOR_MEM_DELAY 10

/**
 * Return the N least-significant bits from integer K using a bit mask
 * @param the integer to be parsed
 * @param the number of bits to be parsed
 * @return the N least-significant bits from K
 */
#define GET_LS_BITS(k, n) ((k) & ((1 << (n)) - 1))
/**
 * Return the bits from integer K starting at N and ending at M using a bit
 * mask
 * @param the integer to be parsed
 * @param the index of the starting bit to be parsed
 * @param the index of the ending bit to be parsed
 * @return a section of bits from K
 */
#define GET_MID_BITS(k, m, n) GET_LS_BITS((k) >> (m), ((n) - (m)))

/**
 * Return the bits from integer K starting at N and ending at M using a bit
 * mask, but sign-extends the result. This is required to parse immediates.
 * @param the integer to be parsed
 * @param the index of the starting bit to be parsed
 * @param the index of the ending bit to be parsed
 * @return a section of bits from K
 */
// clang-format off
#define GET_BITS_SIGN_EXTEND(k, m, n) \
  ({\
    int _f = GET_MID_BITS(k, m, n); \
    int _w = (n) - (m) - (1); \
    _f = (_f & (1 << (_w - 1))) ? (_f | (-1 << _w)) : _f; \
  })
// clang-format on

#endif /* DEFINITIONS_H_INCLUDED */
