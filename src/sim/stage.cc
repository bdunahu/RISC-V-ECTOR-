#include "stage.h"

Stage::Stage(Stage *next) {
	this->next = next;
}

std::array<int, GPR_NUM> Stage::gprs;
int Stage::pc;
Storage *Stage::storage;
