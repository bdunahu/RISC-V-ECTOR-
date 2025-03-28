#ifndef ID_H
#define ID_H
#include "instrDTO.h"
#include "response.h"
#include "stage.h"

class ID : public Stage
{
  public:
	/**
	 * Constructor.
	 * @param The next stage in the pipeline.
	 * @return A newly allocated ID object.
	 */
	ID(Stage *next);

	Response advance(InstrDTO &i) override;
};

#endif /* ID_D_INCLUDED */
