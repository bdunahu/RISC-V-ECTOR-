#include "if.h"
#include "accessor.h"
#include "instrDTO.h"
#include "response.h"

Response IF::advance(InstrDTO &i)
{
	Response r;
	signed int bits;

	r = this->storage->read_word(this->id, this->pc, bits);
	if (r == OK) {
		++this->pc;
		i.set_if_cycle(this->clock_cycle);
		i.set_instr_bits(bits);
	}

	return r;
}
