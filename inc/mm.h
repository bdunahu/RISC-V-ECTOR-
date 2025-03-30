#ifndef MM_H
#define MM_H
#include "instrDTO.h"
#include "response.h"
#include "stage.h"

class MM : public Stage
{
  public:
	/**
	 * Constructor.
	 * @param The next stage in the pipeline.
	 * @return A newly allocated MM object.
	 */
	MM(Stage *next);

	InstrDTO *advance(Response p) override;
};

#endif /* MM_H_INCLUDED */
