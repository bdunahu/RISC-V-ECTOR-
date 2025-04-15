#ifndef IF_H
#define IF_H
#include "accessor.h"
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

	std::vector<int> stage_info() override;

  private:
	void advance_helper() override;
};

#endif /* IF_H_INCLUDED */
