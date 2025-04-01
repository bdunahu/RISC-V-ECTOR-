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

enum CC {
	GT,
	EQ,
	UF,
	OF,
};

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
	 * A boilerplate version is provided in stage.cc.
	 *
	 * @param a response, indicating whether or not the parent pipe stage is
	 * ready to accept a new instruction object next cycle.
	 * @return a DTO object containing the next instruction to be processed.
	 *
	 * Must set the status to WAIT when the current instruction is evicted..
	 */
	virtual InstrDTO *advance(Response p);

	/* The following methods are made public so that they may be tested, and are
	 * not to be called from outside classes during standard execution.
	 */

	/**
	 * Gets the bit in the condition code register correspondng to `c`.
	 * @param the condition code to retrieve,
	 */
	bool get_condition(CC c);
	/**
	 * Sets the bit in the condition code register corresponding to `c`.
	 * @param the condition code to set.
	 * @param the truthy value to set it to.
	 */
	void set_condition(CC c, bool v);

	/**
	 * Sets the value of the PC register.
	 */
	void set_pc(unsigned int pc);

	/**
	 * Squashes the pipeline.
	 */
	void squash();

  protected:
	/**
	 * The function expected to do the majority of the work.
	 *
	 * Must set the status to OK when an operation is done.
	 * Must set the status to STALLED when an operation cannot be completed the
	 * current cycle.
	 */
	virtual void advance_helper() = 0;
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
