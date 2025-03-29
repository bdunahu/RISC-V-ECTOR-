#ifndef EX_H
#define EX_H
#include "instrDTO.h"
#include "response.h"
#include "stage.h"

class EX : public Stage
{
  public:
	/**
	 * Constructor.
	 * @param The next stage in the pipeline.
	 * @return A newly allocated EX object.
	 */
	EX(Stage *next);

	Response advance(InstrDTO &i, Response p) override;
};

#endif /* EX_H_INCLUDED */
