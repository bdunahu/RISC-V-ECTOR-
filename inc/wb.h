#ifndef WB_H
#define WB_H
#include "response.h"
#include "stage.h"
#include "instrDTO.h"

class WB : public Stage
{
  public:
	/**
	 * Constructor.
	 * @param The next stage in the pipeline.
	 * @return A newly allocated WB object.
	 */
	WB(Stage *next);

	Response advance(InstrDTO &i) override;
};

#endif /* WB_H_INCLUDED */
