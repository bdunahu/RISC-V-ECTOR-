#ifndef WB_H
#define WB_H
#include "response.h"
#include "stage.h"
#include "instrDTO.h"

class WB : public Stage
{
  public:
	using Stage::Stage;

	Response advance(InstrDTO &i) override;
};

#endif /* WB_H_INCLUDED */
