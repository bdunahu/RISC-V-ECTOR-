#ifndef IF_H
#define IF_H
#include "response.h"
#include "stage.h"

class IF : public Stage
{
  public:
	using Stage::Stage;

	Response advance();
};

#endif /* IF_H_INCLUDED */
