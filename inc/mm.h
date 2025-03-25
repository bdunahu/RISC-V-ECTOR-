#ifndef MM_H
#define MM_H
#include "response.h"
#include "stage.h"

class MM : public Stage
{
  public:
	using Stage::Stage;

	Response advance();
};

#endif /* MM_H_INCLUDED */
