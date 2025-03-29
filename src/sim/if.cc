#include "if.h"
#include "accessor.h"
#include "instrDTO.h"
#include "response.h"
#include "stage.h"

IF::IF(Stage *stage) : Stage(stage) { this->id = FETCH; }

Response IF::advance(InstrDTO &i, Response p)
{
	Response r;
	signed int bits;

	r = this->storage->read_word(this->id, this->pc, bits);
	if (r == OK) {
		++this->pc;
		i.set_time_of(this->id, this->clock_cycle);
		i.set_instr_bits(bits);
	}

	return r;
}
