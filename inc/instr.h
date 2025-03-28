#ifndef INSTR_H
#define INSTR_H
#include <functional>
#include <iostream>
#include <map>

enum Mnemonic {
	ADD,
	SUB,
	MUL,
	QUOT,
	REM,
	SFTR,
	SFTL,
	AND,
	OR,
	NOT,
	XOR,
	ADDV,
	SUBV,
	MULV,
	DIVV,
	CMP,
	CEV,
	LOAD,
	LOADV,
	ADDI,
	SUBI,
	SFTRI,
	SFTLI,
	ANDI,
	ORI,
	XORI,
	STORE,
	STOREV,
	JMP,
	JRL,
	JAL,
	BEQ,
	BGT,
	BUF,
	BOF,
	PUSH,
	POP,
};

std::ostream &operator<<(std::ostream &os, Mnemonic a);

namespace instr
{
// clang-format off
	extern const std::map<unsigned int, Mnemonic> mnemonic_map;
	extern const std::map<Mnemonic, std::function<void(signed int &s1, signed int &s2, signed int &s3)>> instr_map;
// clang-format on
} // namespace instr

#endif /* INSTR_H_INCLUDED */
