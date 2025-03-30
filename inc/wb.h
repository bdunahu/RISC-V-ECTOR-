#ifndef WB_H
#define WB_H
#include "instrDTO.h"
#include "response.h"
#include "stage.h"

class WB : public Stage
{
  public:
	/**
	 * Constructor.
	 * @param The next stage in the pipeline.
	 * @return A newly allocated WB object.
	 */
	WB(Stage *next);

	InstrDTO *advance(Response p) override;
};

#endif /* WB_H_INCLUDED */
