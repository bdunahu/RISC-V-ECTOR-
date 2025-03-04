#ifndef STORAGE_H
#define STORAGE_H
#include <array>
#include <vector>

class Storage
{
  public:
	int **view(int base) { return nullptr; }
	virtual bool store();

	std::vector<std::array<signed int, 4>> address_space;
};

#endif /* STORAGE_H_INCLUDED */
