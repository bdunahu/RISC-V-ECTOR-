#ifndef DUM_H
#define DUM_H
#include "instrDTO.h"
#include "response.h"
#include "stage.h"

/**
 * Don't underestimate mocks (the DUM pipe stage).
 */
class DUM : public Stage
{
  public:
	/**
	 * Constructor.
	 * @param The next stage in the pipeline.
	 * @return A newly allocated DUM object.
	 */
	DUM(Stage *next);

	InstrDTO *advance(Response p) override;

	void set_curr_instr(InstrDTO *);

  private:
	void advance_helper() override;
};

#endif /* DUM_H_INCLUDED */
