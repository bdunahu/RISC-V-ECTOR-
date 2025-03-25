#ifndef STAGE_H
#define STAGE_H
#include "definitions.h"
#include "response.h"
#include "storage.h"
#include <array>

class Stage
{
  public:
	Stage(Stage *next);
	virtual ~Stage() = default;
	/**
	 * Advances this stage by a single clock cycle.
	 * @return a response, indicating whether this pipeline stage is stalled,
	 * busy, or done.
	 */
	virtual Response advance() = 0;

  protected:
	/**
	 * The shared pool of general-purpose integer registers.
	 */
	static std::array<int, GPR_NUM> gprs;
	/**
	 * The address of the currently executing instruction.
	 */
	static int pc;
	/**
	 * A pointer to the next stage in the pipeline.
	 */
	Stage *next;
	/**
	 * A pointer to the top-level storage device.
	 */
	static Storage *storage;
	/**
	 * A flag indicating whether pipelining should be used.
	 */
	bool is_pipelined;
};

#endif /* STAGE_H_INCLUDED */
