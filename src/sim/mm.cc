#include "mm.h"
#include "accessor.h"
#include "instrDTO.h"
#include "response.h"
#include "stage.h"

MM::MM(Stage *stage) : Stage(stage) { this->id = MEM; } 

Response MM::advance(InstrDTO &next_instr, Response p)
{
	return OK;
}
