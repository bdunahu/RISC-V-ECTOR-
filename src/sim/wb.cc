#include "wb.h"
#include "accessor.h"
#include "instrDTO.h"
#include "response.h"
#include "stage.h"

WB::WB(Stage *stage) : Stage(stage) { this->id = WRITE; }

void WB::advance_helper() {}
