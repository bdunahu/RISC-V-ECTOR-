#include "dram.h"
#include "definitions.h"
#include "response.h"
#include <algorithm>

Dram::Dram(int lines, int delay)
{
	this->data = new std::vector<std::array<signed int, LINE_SIZE>>;
	this->data->resize(lines);
	this->delay = delay;
	this->lower = nullptr;
}

Dram::~Dram() { delete this->data; }

Response *Dram::write(Accessor accessor, signed int data, int address)
{
	struct Response *r = new Response();
	int line = address / LINE_SIZE;
	int word = address % LINE_SIZE;

	this->data->at(line).at(word) = data;

	r->status = OK;
	return r;
}

Response *Dram::read(Accessor accessor, int address) { return nullptr; }
