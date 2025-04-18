#include "stage.h"
#include <array>
#include <deque>

Stage::Stage(Stage *next)
{
	this->next = next;
	this->curr_instr = nullptr;
	this->status = WAIT;
}

Stage::~Stage() { delete this->next; };

std::array<int, GPR_NUM> Stage::gprs;
std::array<int, V_NUM> Stage::vrs;
std::deque<signed int> Stage::checked_out;
unsigned int Stage::pc;
Storage *Stage::storage;
bool Stage::is_pipelined;
bool Stage::is_empty;
int Stage::clock_cycle;

bool Stage::get_condition(CC c) { return (this->gprs[3] >> c) & 1; }

InstrDTO *Stage::advance(Response p)
{
	InstrDTO *r = nullptr;
	InstrDTO *s = nullptr;
	Response n;

	// std::cout << "advance: " << this->id << ": " << this->curr_instr << "?: "
	// << p << ": " << this->checked_out.size() << ": "; if (curr_instr)
	// 	std::cout << curr_instr->get_mnemonic();
	// for (long unsigned int i = 0; i < this->checked_out.size(); ++i)
	// 	std::cout << this->checked_out[i] << " ";
	// std::cout << std::endl;
	if (this->curr_instr && this->curr_instr->is_squashed() == 1)
		this->status = OK;
	if (this->curr_instr && this->status != OK) {
		this->advance_helper();
	}
	if (this->status == OK && p == WAIT && this->curr_instr) {
		// mutual consent
		r = new InstrDTO(*this->curr_instr);
		delete curr_instr;
		curr_instr = nullptr;
		this->status = WAIT;
	}

	n = (p != WAIT || this->status != WAIT) ? STALLED : WAIT;
	s = this->next->advance(n);
	if (s)
		this->curr_instr = s;
	return r;
}

std::vector<int> Stage::stage_info()
{
	std::vector<int> info;
	if (this->curr_instr) {
		info.push_back(this->curr_instr->get_mnemonic());
		info.push_back(this->curr_instr->get_pc());
		info.push_back(this->curr_instr->get_s1());
		info.push_back(this->curr_instr->get_s2());
		info.push_back(this->curr_instr->get_s3());
	}
	return info;
}

void Stage::set_condition(CC c, bool v)
{
	if (v)
		this->gprs[3] = this->gprs[3] | 1 << c;
	else
		this->gprs[3] = this->gprs[3] & ~(1 << c);
}

signed int Stage::dereference_register(signed int v)
{
	signed int r;

	if (v < 0 || v >= GPR_NUM + V_NUM) {
		throw std::out_of_range(
			"instruction tried to access register which does not exist");
	}

	r = (v >= GPR_NUM) ? this->vrs[v % GPR_NUM] : this->gprs[v];
	return r;
}

void Stage::store_register(signed int v, signed int d)
{
	if (v < 0 || v >= GPR_NUM + V_NUM) {
		throw std::out_of_range(
			"instruction tried to access register which does not exist");
	}

	if (v >= GPR_NUM)
		this->vrs[v % GPR_NUM] = d;
	else
		this->gprs[v] = d;
}

bool Stage::is_checked_out(signed int r)
{
	return std::find(this->checked_out.begin(), this->checked_out.end(), r) !=
		   this->checked_out.end();
}

void Stage::squash()
{
	if (curr_instr) {
		this->curr_instr->squash();
		this->status = OK;
	}
	if (this->next) {
		this->next->squash();
	}
}
