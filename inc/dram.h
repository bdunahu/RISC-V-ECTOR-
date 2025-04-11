#ifndef DRAM_H
#define DRAM_H
#include "component.h"
#include "definitions.h"
#include "storage.h"
#include <functional>
#include <ostream>

class Dram : public Storage
{
  public:
	/**
	 * Constructor.
	 * @param The number of clock cycles each access takes.
	 * @return A new memory object.
	 */
	Dram(int delay);
	~Dram();

	int
	write_word(Component, signed int, int) override;
	int
	write_line(Component, std::array<signed int, LINE_SIZE>, int) override;
	int
	read_word(Component, int, signed int &) override;
	int
	read_line(Component, int, std::array<signed int, LINE_SIZE> &) override;

	/**
	 * TODO This will accept a file at a later date.
	 */
	void
	load(std::vector<signed int> program);

  private:
	/**
	 * Helper for all access methods.
	 * Calls `request_handler` when `component` is allowed to complete its
	 * request cycle.
	 * @param the source making the request
	 * @param the address to write to
	 * @param the function to call when an access should be completed
	 * @return 1 if the access completed successfully, 0 otherwise
	 */
	int
	process(
		Component component, int address, std::function<void(int line, int word)> request_handler);
	/**
	 * Returns OK if `component` is allowed to complete its request this cycle.
	 * Handles wait times, side door, and setting the current component this
	 * storage is serving.
	 * @param the component asking for a resource
	 * @return 1 if the access can be completed this function call, 0 otherwise
	 */
	int
	is_access_cleared(Component component);
};

#endif /* DRAM_H_INCLUDED */
