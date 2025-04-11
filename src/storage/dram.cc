#include "dram.h"
#include "definitions.h"
#include <algorithm>
#include <bits/stdc++.h>
#include <bitset>
#include <iostream>
#include <iterator>
#include <utils.h>

Dram::Dram(int delay)
{
	this->data = new std::vector<std::array<signed int, LINE_SIZE>>;
	this->data->resize(MEM_LINES);
	this->delay		 = delay;
	this->is_waiting = false;
	this->lower		 = nullptr;
	this->requester	 = IDLE;
	this->wait_time	 = this->delay;
}

Dram::~Dram() { delete this->data; }

int
Dram::write_line(Accessor accessor, std::array<signed int, LINE_SIZE> data_line, int address)
{
	return process(accessor, address, [&](int line, int word) {
		(void)word;
		this->data->at(line) = data_line;
	});
}

int
Dram::write_word(Accessor accessor, signed int data, int address)
{
	return process(
		accessor, address, [&](int line, int word) { this->data->at(line).at(word) = data; });
}

// TODO requires testing
int
Dram::read_line(Accessor accessor, int address, std::array<signed int, LINE_SIZE> &data_line)
{
	return process(accessor, address, [&](int line, int word) {
		(void)word;
		data_line = this->data->at(line);
	});
}

int
Dram::read_word(Accessor accessor, int address, signed int &data)
{
	return process(
		accessor, address, [&](int line, int word) { data = this->data->at(line).at(word); });
}

// TODO load a file instead and test this method
void
Dram::load(std::vector<signed int> program)
{
	unsigned long i;
	for (i = 0; i < program.size(); ++i) {
		int line, word;
		get_memory_index(i, line, word);
		this->data->at(line).at(word) = program[i];
	}
}

int
Dram::process(
	Accessor accessor, int address, std::function<void(int line, int word)> request_handler)
{
	int r;
	r = this->is_access_cleared(accessor);
	if (r) {
		int line, word;
		get_memory_index(address, line, word);
		request_handler(line, word);
	}
	return r;
}

int
Dram::is_access_cleared(Accessor accessor)
{
	/* Do this first--then process the first cycle immediately. */
	if (accessor == SIDE)
		return 1;
	else {
		if (this->requester == IDLE)
			this->requester = accessor;
		if (this->requester == accessor) {
			if (this->wait_time == 0) {
				this->requester = IDLE;
				this->wait_time = delay;
				return 1;
			} else {
				--this->wait_time;
			}
		}
	}
	return 0;
}
