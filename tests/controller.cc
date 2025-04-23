#include "controller.h"
#include "cache.h"
#include "dram.h"
#include "ex.h"
#include "id.h"
#include "if.h"
#include "mm.h"
#include "wb.h"
#include <algorithm>
#include <catch2/catch_test_macros.hpp>
#include <vector>

class ControllerPipeFixture
{
  public:
	ControllerPipeFixture()
	{
		this->d = new Dram(1);
		this->c = new Cache(this->d, 5, 0, 0);

		IF *f = new IF(nullptr);
		ID *d = new ID(f);
		EX *e = new EX(d);
		MM *m = new MM(e);
		WB *w = new WB(m);
		this->stage_num = 5;

		this->ct = new Controller(w, this->c, true);
	}
	~ControllerPipeFixture()
	{
		delete this->ct;
		delete this->c;
	};
	void fill_pipe()
	{
		InstrDTO *i = nullptr;
		int j;
		for (j = 0; j < this->stage_num + 1; ++j) {
			i = this->ct->advance(READY);
			REQUIRE(i == nullptr);
		}
	}
	int stage_num;
	Cache *c;
	Dram *d;
	Controller *ct;
};
