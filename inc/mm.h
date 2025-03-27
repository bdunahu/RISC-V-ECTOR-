#ifndef MM_H
#define MM_H
#include "instrDTO.h"
#include "response.h"
#include "stage.h"

class MM : public Stage
{
  public:
	using Stage::Stage;

	Response advance(InstrDTO &i) override;
};

#endif /* MM_H_INCLUDED */
