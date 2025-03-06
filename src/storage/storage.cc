#include "storage.h"
#include "definitions.h"
#include <algorithm>

std::vector<std::array<signed int, LINE_SIZE>>
Storage::view(int base, int lines)
{
	base = (base / LINE_SIZE) * LINE_SIZE;
	std::vector<std::array<signed int, LINE_SIZE>> ret(lines + 1);
	std::copy(
		this->data->begin() + base, this->data->begin() + base + lines,
		ret.begin());
	return ret;
}

void Storage::do_write(signed data, int address)
{
	int line = address / LINE_SIZE;
	int word = address % LINE_SIZE;

	this->servicing = IDLE;
	this->data->at(line).at(word) = data;
}
