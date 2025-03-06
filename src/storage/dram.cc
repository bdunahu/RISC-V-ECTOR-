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
	this->servicing = IDLE;
}

Dram::~Dram() { delete this->data; }

Response *Dram::write(Accessor accessor, signed int data, int address)
{
	struct Response *r = new Response();
	r->status = WAIT;

	/* Do this first--then process the first cycle immediately. */
	if (this->servicing == IDLE) {
		this->servicing = accessor;
		this->wait_time = delay;
	}

	if (this->servicing == accessor) {
		if (this->wait_time == 0) {
			int line = address / LINE_SIZE;
			int word = address % LINE_SIZE;

			this->servicing = IDLE;
			this->data->at(line).at(word) = data;
			r->status = OK;
		} else {
			--this->wait_time;
		}
	}

	return r;
}

Response *Dram::read(Accessor accessor, int address) { return nullptr; }
