#include "storage.h"
#include <algorithm>

std::vector<std::array<signed int, 4>> Storage::view(int base, int lines)
{
	base = (base / 4) * 4;
	std::vector<std::array<signed int, 4>> ret(lines + 1);
	std::copy(
		this->data->begin() + base, this->data->begin() + base + lines,
		ret.begin());
	return ret;
}
