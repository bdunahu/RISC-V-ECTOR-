#include "mm.h"
#include "accessor.h"
#include "instrDTO.h"
#include "response.h"
#include "stage.h"

MM::MM(Stage *stage) : Stage(stage) { this->id = MEM; } 

void MM::advance_helper() {}
