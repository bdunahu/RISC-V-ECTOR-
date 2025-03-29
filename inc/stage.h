#ifndef STAGE_H
#define STAGE_H
#include "definitions.h"
#include "instrDTO.h"
#include "response.h"
#include "storage.h"
#include "accessor.h"
#include <array>

class Stage
{
  public:
	/**
	 * Constructor.
	 * @param The next stage in the pipeline.
	 * @return A newly allocated stage object.
	 */
	Stage(Stage *next);
	virtual ~Stage() = default;
	/**
	 * Advances this stage by a single clock cycle.
	 * @param a DTO object containing various information about an instruction
	 * moving through the pipeline.
	 * @return a response, indicating whether this pipeline stage is stalled,
	 * busy, or done.
	 */
	virtual Response advance(InstrDTO &i) = 0;

  protected:
	/**
	 * The name of the pipeline stage.
	 */
	Accessor id;
	/**
	 * The shared pool of general-purpose integer registers.
	 */
	static std::array<signed int, GPR_NUM> gprs;
	/**
	 * The shared pool of general-purpose vector registers.
	 */
	static std::array<signed int, V_NUM> vrs;
	/**
	 * The address of the currently executing instruction.
	 */
	static unsigned int pc;
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
	static bool is_pipelined;
	/**
	 * The current clock cycle.
	 */
	static int clock_cycle;
};

#endif /* STAGE_H_INCLUDED */
