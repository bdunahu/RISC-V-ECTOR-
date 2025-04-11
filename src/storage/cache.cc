#include "cache.h"
#include "definitions.h"
#include "utils.h"
#include <bits/stdc++.h>
#include <iostream>
#include <iterator>

Cache::Cache(Storage *lower, int delay)
{
	this->data = new std::vector<std::array<signed int, LINE_SIZE>>;
	this->data->resize(L1_CACHE_LINES);
	this->delay		 = delay;
	this->is_waiting = false;
	this->lower		 = lower;
	this->meta.fill({-1, -1});
	this->requester = IDLE;
	this->wait_time = this->delay;
}

Cache::~Cache()
{
	delete this->lower;
	delete this->data;
}

int
Cache::write_word(Accessor accessor, signed int data, int address)
{
	return process(accessor, address, [&](int index, int offset) {
		this->data->at(index).at(offset) = data;
		this->meta[index].at(1)			 = 1;
	});
}

int
Cache::write_line(Accessor accessor, std::array<signed int, LINE_SIZE> data_line, int address)
{
	return process(accessor, address, [&](int index, int offset) {
		(void)offset;
		this->data->at(index)	= data_line;
		this->meta[index].at(1) = 1;
	});
}

// TODO: tests for multi level cache
int
Cache::read_line(Accessor accessor, int address, std::array<signed int, LINE_SIZE> &data_line)
{
	return process(accessor, address, [&](int index, int offset) {
		(void)offset;
		data_line = this->data->at(index);
	});
}

int
Cache::read_word(Accessor accessor, int address, signed int &data)
{
	return process(
		accessor, address, [&](int index, int offset) { data = this->data->at(index).at(offset); });
}

int
Cache::process(
	Accessor accessor, int address, std::function<void(int index, int offset)> request_handler)
{
	int r;
	r = this->is_access_cleared(accessor, address);
	if (r) {
		int tag, index, offset;
		get_cache_fields(address, &tag, &index, &offset);
		request_handler(index, offset);
	}
	return r;
}

int
Cache::is_access_cleared(Accessor accessor, int address)
{
	int r;
	r = 0;
	/* Do this first--then process the first cycle immediately. */
	if (this->requester == IDLE)
		this->requester = accessor;
	if (this->requester == accessor) {
		handle_miss(address);
		if (this->is_waiting)
			r = 0;
		else if (this->wait_time == 0) {
			this->requester = IDLE;
			this->wait_time = delay;
			r = 1;
		} else {
			--this->wait_time;
		}
	}
	return r;
}

void
Cache::handle_miss(int expected)
{
	int r, q, tag, index, offset;
	std::array<signed int, LINE_SIZE> *actual;
	std::array<int, 2> *meta;

	get_cache_fields(expected, &tag, &index, &offset);
	r	   = 1;
	meta   = &this->meta.at(index);
	actual = &this->data->at(index);

	if (meta->at(0) != tag) {
		r = 0;
		// address not in cache
		if (meta->at(1) >= 0) {
			// occupant is dirty
			// writing line to DRam in case of dirty cache eviction
			q = this->lower->write_line(
				L1CACHE, *actual,
				((index << LINE_SPEC) + (meta->at(0) << (L1_CACHE_LINE_SPEC + LINE_SPEC))));
			if (q) {
				meta->at(1) = -1;
			}
		} else {
			q = this->lower->read_line(L1CACHE, expected, *actual);
			if (q) {
				meta->at(0) = tag;
			}
		}
	}

	this->is_waiting = r ? false : true;
}

std::array<std::array<int, 2>, L1_CACHE_LINES>
Cache::get_meta() const
{
	std::array<std::array<int, 2>, L1_CACHE_LINES> ret;
	std::copy(std::begin(this->meta), std::end(this->meta), std::begin(ret));
	return ret;
}
