#ifndef IF_H
#define IF_H
#include "instrDTO.h"
#include "response.h"
#include "stage.h"

class IF : public Stage
{
  public:
	/**
	 * Constructor.
	 * @param The next stage in the pipeline.
	 * @return A newly allocated IF object.
	 */
	IF(Stage *next);

	InstrDTO *advance(Response p) override;

  private:
	void advance_helper() override;
};

#endif /* IF_H_INCLUDED */
