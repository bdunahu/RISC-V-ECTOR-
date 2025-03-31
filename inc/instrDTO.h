#ifndef INSTRDTO_H
#define INSTRDTO_H
#include "accessor.h"
#include "instr.h"
#include <functional>
#include <string>
#include <unordered_map>

class InstrDTO
{
  public:
	/**
	 * Constructor.
	 */
	InstrDTO();
	~InstrDTO() = default;

	/**
	 * @return hist entry for Accessor
	 */
	int get_time_of(Accessor);
	/**
	 * @return id_cycle
	 */
	int get_id_cycle();
	/**
	 * @return instr_bits
	 */
	signed int get_instr_bits();
	/**
	 * @return s1
	 */
	signed int get_s1();
	/**
	 * @return s2
	 */
	signed int get_s2();
	/**
	 * @return s3
	 */
	signed int get_s3();
	/**
	 * @return the mnemonic of the instruction
	 */
	Mnemonic get_mnemonic();
	/**
	 * @return the type of the instruction
	 */
	Type get_type();

	/**
	 * @param set hist key
	 */
	void set_time_of(Accessor, int);
	/**
	 * @param instr_bits
	 */
	void set_instr_bits(signed int);
	/**
	 * @param s1
	 */
	void set_s1(signed int);
	/**
	 * @param s2
	 */
	void set_s2(signed int);
	/**
	 * @param s3
	 */
	void set_s3(signed int);
	/**
	 * @param the mnemonic of the instruction
	 */
	void set_mnemonic(Mnemonic);
	
	/**
	 * @param the type of the instruction
	 */
	void set_type(Type);

  private:
	/**
	 * The clock cycle each stage finished an operation.
	 */
	std::unordered_map<Accessor, int> hist;
	/**
	 * The raw bits encoding the instruction.
	 */
	signed int instr_bits;
	/**
	 * Slots in this instruction, for storing temporary registers, immediates,
	 * or other.
	 * Some instruction types may use these differently.
	 * The `oper` function is in charge of knowing how to parse these.
	 */
	signed int s1;
	signed int s2;
	signed int s3;
	/**
	 * The mnemonic of the operation.
	 */
	Mnemonic mnemonic;

	/**
	 * Type of the instruction
	 */
	Type type;
};

#endif /* INSTRDTO_H_INCLUDED */
