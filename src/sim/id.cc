#include "id.h"
#include "accessor.h"
#include "instrDTO.h"
#include "response.h"
#include "stage.h"

ID::ID(Stage *stage) : Stage(stage) { this->id = DCDE; }

Response ID::advance(InstrDTO &i) { return OK; }
