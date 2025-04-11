#include "component.h"
#include <iostream>

std::ostream &operator<<(std::ostream &os, Component c)
{
	const std::string nameC[] = {
		"CONT", "WRITE", "MEM", "EXEC", "DCDE", "FETCH", "CACHE", "VOID",
	};
	return os << nameC[c];
}
