#ifndef ID_H
#define ID_H
#include "instr.h"
#include "instrDTO.h"
#include "response.h"
#include "stage.h"

class ID : public Stage
{
  public:
	/**
	 * Constructor.
	 * @param The next stage in the pipeline.
	 * @return A newly allocated ID object.
	 */
	ID(Stage *next);
	using Stage::advance;

	/* The following methods are made public so that they may be tested, and are
	 * not to be called from outside classes during standard execution.
	 */

	/**
	 * Facilitates register checkout and data hazard management.
	 * It does this by checking that the register passed in is not currently
	 * checked out. If true, then replaces r with the value of the register and
	 * returns OK. If false, returns STALLED.
	 *
	 * @param the registers number, to be dereferenced.
	 * @return OK if `r` is not checked out, STALLED otherwise.
	 */
	Response read_guard(signed int &r);
	/**
	 * Facilitates register checkout and data hazard management.
	 * Checks out a register and returns it.
	 *
	 * @param the registers number, to be dereferenced and checked out.
	 */
	void write_guard(signed int &r);

  private:
	void advance_helper() override;
	/**
	 * Helper for `get_instr_fields`
	 * Attempts to parse and dereference instruction arguments. Uses read and
	 * write guards to prevent RAW conflicts.
	 *
	 * @param the resulting first field. To call this function properly, this
	 * field must contain the section of the instruction to be parsed.
	 * @param the resulting second field.
	 * @param the resulting third field.
	 */
	/**
	 * Parse an instruction into a type, opcode, and fields. If the type is
	 * invalid, only the type field will be set.
	 *
	 * This method is marked public so it may be tested, and is not used outside
	 * of this class during normal execution.
	 *
	 * @param the resulting first field, which varies per type. To call this
	 * function properly, this field must contain the full instruction bytes on
	 * function entry.
	 * @param the resulting second field, which varies per type.
	 * @param the resulting third field, which varies per type.
	 * @param the resulting mnemonic.
	 */
	void get_instr_fields(signed int &s1, signed int &s2, signed int &s3, Mnemonic &m, Type &t);
	void decode_R_type(signed int &s1, signed int &s2, signed int &s3);
	void decode_I_type(signed int &s1, signed int &s2, signed int &s3, Mnemonic &m);
	void decode_J_type(signed int &s1, signed int &s2);
	/**
	 * Helper for `get_instr_fields`.
	 * Given a raw instruction, returns the mnemonic and type.
	 * This operation will destroy the original arguments.
	 * @param the raw bits to parse.
	 * @param the resulting mnemonic.
	 * @param the resulting type.
	 */
	void split_instr(signed int &raw, unsigned int &type, Mnemonic &m);
};

#endif /* ID_H_INCLUDED */
