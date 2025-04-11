#ifndef COMPONENT_H
#define COMPONENT_H
#include <iostream>

enum Component {
	CONT,
	WRITE,
	MEM,
	EXEC,
	DCDE,
	FETCH,
	CACHE,
	VOID,
};

std::ostream &
operator<<(std::ostream &os, Component c);

#endif /* COMPONENT_H_INCLUDED */
