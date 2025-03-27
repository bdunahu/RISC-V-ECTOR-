#ifndef EX_H
#define EX_H
#include "instrDTO.h"
#include "response.h"
#include "stage.h"

class EX : public Stage
{
  public:
	using Stage::Stage;

	Response advance(InstrDTO &i) override;
};

#endif /* EX_H_INCLUDED */
