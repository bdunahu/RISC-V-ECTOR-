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

	Response advance(InstrDTO &i, Response p) override;

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
	void get_instr_fields(
		signed int &s1, signed int &s2, signed int &s3, Mnemonic &m);

  private:
	/**
	 * Helper for `get_instr_fields`.
	 * Given a raw instruction, returns the mnemonic and type.
	 * This operation will destroy the original arguments.
	 * @param the raw bits to parse.
	 * @param the resulting mnemonic.
	 * @param the resulting type.
	 */
	void split_instr(signed int &raw, unsigned int &type, Mnemonic &m);
	/**
	 * Facilitates checking out a register by dereferencing the register
	 specified by the first parameter. Registers that are checked out cannot be
	 checked out until they are checked in.
	 * @param
	 * @return the contents of the register.
	 */
	Response dereference_register(signed int &);
};

#endif /* ID_D_INCLUDED */
