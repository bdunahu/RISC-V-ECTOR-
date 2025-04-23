#ifndef DUM_H
#define DUM_H

#include "instrDTO.h"
#include "response.h"
#include "stage.h"

/**
 * Don't underestimate mocks (the DUM pipe stage).
 */
class DUM : public Stage
{
  public:
	using Stage::Stage;

	InstrDTO *advance(Response p) override
	{
		InstrDTO *r = nullptr;

		if (this->curr_instr && p == READY) {
			r = new InstrDTO(*this->curr_instr);
			delete this->curr_instr;
			curr_instr = nullptr;
		}

		return r;
	}

	void set_curr_instr(InstrDTO *d) { this->curr_instr = d; };

  private:
	void advance_helper() override{};
};

#endif /* DUM_H_INCLUDED */
