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
	using Stage::advance;

  private:
	void advance_helper() override;
	/**
	 * Performs the actual work of storing into a register.
	 */
	void write_handler();
	/**
	 * Performs the actual work of processing a jump instruction.
	 */
	void jump_handler();
	/**
	 * @return true if the current instruction is an R or I type and is not a
	 * STORE.
	 */
	bool should_write();
	/**
	 * @return true if the current instruction is a J type and is not a push.
	 * STORE.
	 */
	bool should_jump();
};

#endif /* WB_H_INCLUDED */
