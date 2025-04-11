#ifndef COMPONENT_H
#define COMPONENT_H
#include <iostream>

enum Component {
	IDLE,
	WRITE,
	MEM,
	EXEC,
	DCDE,
	FETCH,
	L1CACHE,
	SIDE,
};

std::ostream &operator<<(std::ostream &os, Component c);

#endif /* COMPONENT_H_INCLUDED */
