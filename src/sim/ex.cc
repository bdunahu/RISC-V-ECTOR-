#include "ex.h"
#include "accessor.h"
#include "instrDTO.h"
#include "response.h"
#include "stage.h"

EX::EX(Stage *stage) : Stage(stage) { this->id = EXEC; }

Response EX::advance(InstrDTO &i) { return OK; }
