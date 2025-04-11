#ifndef DRAM_H
#define DRAM_H
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
	write_word(Accessor accessor, signed int data, int address) override;
	int
	write_line(
		Accessor accessor, std::array<signed int, LINE_SIZE> data_line, int address) override;
	int
	read_word(Accessor accessor, int address, signed int &data) override;
	int
	read_line(
		Accessor accessor, int address, std::array<signed int, LINE_SIZE> &data_line) override;

	/**
	 * TODO This will accept a file at a later date.
	 */
	void
	load(std::vector<signed int> program);

  private:
	/**
	 * Helper for all access methods.
	 * Calls `request_handler` when `accessor` is allowed to complete its
	 * request cycle.
	 * @param the source making the request
	 * @param the address to write to
	 * @param the function to call when an access should be completed
	 * @return 1 if the access completed successfully, 0 otherwise
	 */
	int
	process(
		Accessor accessor, int address, std::function<void(int line, int word)> request_handler);
	/**
	 * Returns OK if `accessor` is allowed to complete its request this cycle.
	 * Handles wait times, side door, and setting the current accessor this
	 * storage is serving.
	 * @param the accessor asking for a resource
	 * @return 1 if the access can be completed this function call, 0 otherwise
	 */
	int
	is_access_cleared(Accessor accessor);
};

std::ostream &
operator<<(std::ostream &os, const Dram &d);

#endif /* DRAM_H_INCLUDED */
