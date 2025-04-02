#ifndef CONTROLLER_H
#define CONTROLLER_H
#include "instrDTO.h"
#include "response.h"
#include "stage.h"

/**
 * Houses the clock, and acts as the main API to the GUI.
 */
class Controller : public Stage
{
  public:
	/**
	 * Constructor.
	 * @param The stage(s) to interface with.
	 * @param The storage object to use.
	 * @param Whether or not efficient pipelining will be used.
	 * @return A newly allocated controller object.
	 */
	Controller(Stage *stage, Storage *storage, bool is_pipelined);
	InstrDTO *advance(Response p) override;
	
	/**
	 * Direct the simulator to run for `number` clock cycles.
	 * @param the number of clock cycles to run for.
	 */
	void run_for(int number);
	/**
	 * @return the current clock cycle.
	 */
	int get_clock_cycle();
	/**
	 * @return a copy of gprs.
	 */
	std::array<int, GPR_NUM> get_gprs();
	/**
	 * @return the pc.
	 */
	int get_pc();

	void set_gprs(int index, int value);

	void set_pipelined(bool value);

  private:
	void advance_helper() override;
};

#endif /* CONTROLLER_H_INCLUDED */
