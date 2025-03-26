#ifndef IF_H
#define IF_H
#include "instrDTO.h"
#include "response.h"
#include "stage.h"

class IF : public Stage
{
  public:
	using Stage::Stage;

	Response advance(InstrDTO &i) override;

  private:
	/**
	 * The name this pipeline stages uses to access storage.
	 */
	const Accessor id = FETCH;
};

#endif /* IF_H_INCLUDED */
