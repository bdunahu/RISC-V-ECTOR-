#ifndef STAGE_H
#define STAGE_H
#include "accessor.h"
#include "definitions.h"
#include "instrDTO.h"
#include "response.h"
#include "storage.h"
#include <array>
#include <deque>
#include <memory>

class Stage
{
  public:
	/**
	 * Constructor.
	 * @param The next stage in the pipeline.
	 * @return A newly allocated stage object.
	 */
	Stage(Stage *next);
	virtual ~Stage();
	/**
	 * Advances this stage by a single clock cycle.
	 * @param a DTO object containing the next instruction to be processed.
	 * @param a response, indicating whether or not the parent pipe stage is
	 * ready to accept a new instruction object next cycle.
	 * @return a response, indicating whether this pipeline stage is stalling,
	 * busy, or done.
	 */
	virtual InstrDTO *advance(Response p) = 0;

  protected:
	/**
	 * Helper for `check_out`.
	 * Returns true if r are not checked out, false otherwise.
	 * @param a list of register numbers.
	 * @return true if registers are not in checked_out, false otherwise.
	 */
	bool is_checked_out(signed int r);
	/**
	 * Returns the value of the register corresponding to `v`.
	 * @param the register number.
	 * @return the value in the associated register.
	 */
	signed int dereference_register(signed int v);
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
	// TODO fix this comment after writeback stage
	/**
	 * The set of registers currently checked out.
	 */
	static std::deque<signed int> checked_out;
	/**
	 * A pointer to the next stage in the pipeline.
	 */
	Stage *next;
	/**
	 * A pointer to the current instruction this stage is processing.
	 */
	InstrDTO *curr_instr;
	/**
	 * The current status of this stage.
	 */
	Response status;
};

#endif /* STAGE_H_INCLUDED */
