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

	Response advance(InstrDTO &next_instr, Response p) override;

  private:
	/**
	 * Performs a fetch only if a current fetch is not pending. Pending means
	 * that a fetch has completed successfully, but the caller stage in the
	 * pipeline is not ready to receive it. In this case, `curr_instr` is not
	 * the nullptr.
	 * @return STALLED if we are waiting on the storage devices, OK otherwise.
	 */
	void advance_helper();
};

#endif /* IF_H_INCLUDED */
