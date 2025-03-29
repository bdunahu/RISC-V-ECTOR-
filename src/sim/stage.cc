#include "stage.h"
#include "utils.h"
#include <array>
#include <set>

static Logger *global_log = Logger::getInstance();

Stage::Stage(Stage *next) { this->next = next; }

std::array<int, GPR_NUM> Stage::gprs;
std::array<int, V_NUM> Stage::vrs;
std::set<unsigned int> Stage::checked_out;
unsigned int Stage::pc;
Storage *Stage::storage;
bool Stage::is_pipelined;
int Stage::clock_cycle;

Response Stage::check_out(unsigned int &v)
{
	Response r;
	if (this->is_checked_out(v))
		r = STALLED;
	else {
		r = OK;
		v = this->check_out_register(v);
	}
	return r;
}

bool Stage::is_checked_out(unsigned int r)
{
	return this->checked_out.find(r) != this->checked_out.end();
}

signed int Stage::check_out_register(unsigned int v)
{
	signed int r;
	this->checked_out.insert(v);

	r = this->dereference_register(v);
	return r;
}

signed int Stage::dereference_register(unsigned int v)
{
	signed int r;

	if (v >= GPR_NUM + V_NUM) {
		global_log->log(
			ERROR, string_format(
					   "instruction tried to access register %d, which does "
					   "not exist",
					   v));
		exit(EXIT_FAILURE);
	}

	r = (v >= GPR_NUM) ? this->vrs[v % GPR_NUM] : this->gprs[v];
	return r;
}
