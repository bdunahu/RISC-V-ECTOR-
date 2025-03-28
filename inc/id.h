#ifndef ID_H
#define ID_H
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

	Response advance(InstrDTO &i) override;

	/**
	 * Parse an instruction into a type, opcode, and fields. If the type is
	 * invalid, only the type field will be set.
	 * @param the resulting first field, which varies per type. To call this
	 * function properly, this field must contain the full instruction bytes on
	 * function entry.
	 * @param the resulting second field, which varies per type.
	 * @param the resulting third field, which varies per type.
	 * @param the resulting type.
	 * @param the resulting opcode.
	 */
	void get_instr_fields(
		signed int &s1,
		signed int &s2,
		signed int &s3,
		unsigned int &type,
		unsigned int &opcode);
};

#endif /* ID_D_INCLUDED */
