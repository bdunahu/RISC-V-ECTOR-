#include "cache.h"
#include "response.h"
#include <bits/stdc++.h>

Cache::Cache(int lines, Storage *lower, int delay)
{
	this->data = new std::vector<std::array<signed int, 4>>;
	this->data->resize(lines);
	this->lower = lower;
	this->delay = delay;
	this->lower = nullptr;
}

Cache::~Cache() { delete this->data; }

Response *Cache::write(Accessor accessor, signed int data, int address)
{
	return new Response();
}

Response *Cache::read(Accessor accessor, int address) { return nullptr; }
