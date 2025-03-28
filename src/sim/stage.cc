#include "stage.h"

Stage::Stage(Stage *next) {
	this->next = next;
}

std::array<int, GPR_NUM> Stage::gprs;
unsigned int Stage::pc;
Storage *Stage::storage;
bool Stage::is_pipelined;
int Stage::clock_cycle;
