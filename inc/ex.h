#ifndef EX_H
#define EX_H
#include "response.h"
#include "stage.h"

class EX : public Stage
{
  public:
	using Stage::Stage;

	Response advance();
};

#endif /* EX_H_INCLUDED */
