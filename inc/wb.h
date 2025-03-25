#ifndef WB_H
#define WB_H
#include "response.h"
#include "stage.h"

class WB : public Stage
{
  public:
	using Stage::Stage;

	Response advance();
};

#endif /* WB_H_INCLUDED */
