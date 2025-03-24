#ifndef ID_H
#define ID_H
#include "response.h"
#include "stage.h"

class ID : public Stage
{
  public:
	using Stage::Stage;

	Response advance();
};

#endif /* ID_D_INCLUDED */
