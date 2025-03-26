#ifndef ID_H
#define ID_H
#include "instrDTO.h"
#include "response.h"
#include "stage.h"

class ID : public Stage
{
  public:
	using Stage::Stage;

	Response advance(InstrDTO &i) override;
};

#endif /* ID_D_INCLUDED */
