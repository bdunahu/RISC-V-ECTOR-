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

class ControllerPipeFixture
{
  public:
	ControllerPipeFixture()
	{
		this->c = new Cache(new Dram(3), 1);

		IF *f = new IF(nullptr);
		ID *d = new ID(f);
		EX *e = new EX(d);
		MM *m = new MM(e);
		WB *w = new WB(m);

		this->ct = new Controller(w, this->c, true);
	}
	~ControllerPipeFixture()
	{
		delete this->ct;
		delete this->c;
	};

	Cache *c;
	Controller *ct;
};

TEST_CASE_METHOD(
	ControllerPipeFixture,
	"Contructor resets gettable fields",
	"[controller_pipe]")
{
	std::array<int, GPR_NUM> gprs;

	gprs = this->ct->get_gprs();

	CHECK(this->ct->get_clock_cycle() == 0);
	CHECK(std::all_of(
		gprs.begin(), gprs.end(), [](int value) { return value == 0; }));
	// change me later
	CHECK(this->ct->get_pc() == 0);
}
