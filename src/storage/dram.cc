#include "dram.h"
#include "component.h"
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
	this->delay = delay;
	this->lower = nullptr;
	this->requester = VOID;
	this->wait_time = this->delay;
}

Dram::~Dram() { delete this->data; }

int
Dram::write_line(Component component, std::array<signed int, LINE_SIZE> data_line, int address)
{
	return process(component, address, [&](int line, int word) {
		(void)word;
		this->data->at(line) = data_line;
	});
}

int
Dram::write_word(Component component, signed int data, int address)
{
	return process(
		component, address, [&](int line, int word) { this->data->at(line).at(word) = data; });
}

// TODO requires testing
int
Dram::read_line(Component component, int address, std::array<signed int, LINE_SIZE> &data_line)
{
	return process(component, address, [&](int line, int word) {
		(void)word;
		data_line = this->data->at(line);
	});
}

int
Dram::read_word(Component component, int address, signed int &data)
{
	return process(
		component, address, [&](int line, int word) { data = this->data->at(line).at(word); });
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
	Component component, int address, std::function<void(int line, int word)> request_handler)
{
	int r;
	r = this->is_access_cleared(component);
	if (r) {
		int line, word;
		get_memory_index(address, line, word);
		request_handler(line, word);
	}
	return r;
}

int
Dram::is_access_cleared(Component component)
{
	/* Do this first--then process the first cycle immediately. */
	if (this->requester == VOID)
		this->requester = component;
	if (this->requester == component) {
		if (this->wait_time == 0) {
			this->requester = VOID;
			this->wait_time = delay;
			return 1;
		} else {
			--this->wait_time;
		}
	}
	return 0;
}
