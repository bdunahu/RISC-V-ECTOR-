#ifndef STAGE_H
#define STAGE_H
#include "accessor.h"
#include "definitions.h"
#include "instrDTO.h"
#include "response.h"
#include "storage.h"
#include <array>
#include <set>

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
	 * Facilitates register checkout.
	 * It does this by checking that the register passed in is not currently
	 * checked out. If true, then replaces r with the value of the register and
	 * returns OK. If false, returns STALLED.
	 *
	 * @param the registers number, to be dereferenced.
	 * @return OK if `r` is not checked out, STALLED otherwise.
	 */
	Response check_out(unsigned int &r);
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

  private:
	/**
	 * Helper for `check_out`.
	 * Returns true if r are not checked out, false otherwise.
	 * @param a list of register numbers.
	 * @return true if registers are not in checked_out, false otherwise.
	 */
	bool is_checked_out(unsigned int r);
	/**
	 * Helper for `check_out`.
	 * Checks out a single register, and places it in checked_out.
	 * @param a register number.
	 * @return the value in the register.
	 */
	signed int check_out_register(unsigned int r);
	// TODO fix this comment after writeback stage
	/**
	 * Helper for `check_out_register`
	 * @param the register number.
	 * @return the value in the associated register.
	 */
	signed int dereference_register(unsigned int r);
	/**
	 * The set of registers currently checked out.
	 */
	static std::set<unsigned int> checked_out;
};

#endif /* STAGE_H_INCLUDED */
