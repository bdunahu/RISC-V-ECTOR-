#include "component.h"
#include <iostream>

std::ostream &operator<<(std::ostream &os, Component c)
{
	const std::string nameC[] = {
		"IDLE", "WRITE", "MEM", "EXEC", "DCDE", "FETCH", "L1CACHE", "SIDE",
	};
	return os << nameC[c];
}
