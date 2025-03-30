#include "stage.h"
#include "utils.h"
#include <array>
#include <deque>

Stage::Stage(Stage *next)
{
	this->next = next;
	this->curr_instr = nullptr;
	this->status = OK;
	this->checked_out = {};
}

Stage::~Stage() { delete this->next; };

std::array<int, GPR_NUM> Stage::gprs;
std::array<int, V_NUM> Stage::vrs;
std::deque<signed int> Stage::checked_out;
unsigned int Stage::pc;
Storage *Stage::storage;
bool Stage::is_pipelined;
int Stage::clock_cycle;

void Stage::set_condition(CC c, bool v)
{
	if (v)
		this->gprs[3] = this->gprs[3] & 1 << c;
	else
		this->gprs[3] = this->gprs[3] & ~(1 << c);
}

bool Stage::get_condition(CC c) {
	return (this->gprs[3] >> c) & 1;
}

signed int Stage::dereference_register(signed int v)
{
	signed int r;

	if (v < 0 || v >= GPR_NUM + V_NUM) {
		throw std::out_of_range(string_format(
			"instruction tried to access register %d, which does "
			"not exist",
			v));
	}

	r = (v >= GPR_NUM) ? this->vrs[v % GPR_NUM] : this->gprs[v];
	return r;
}

bool Stage::is_checked_out(signed int r)
{
	return std::find(this->checked_out.begin(), this->checked_out.end(), r) !=
		   this->checked_out.end();
}
