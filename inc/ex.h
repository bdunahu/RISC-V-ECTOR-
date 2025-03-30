#ifndef EX_H
#define EX_H
#include "instrDTO.h"
#include "response.h"
#include "stage.h"
#include <unordered_map>

class EX : public Stage
{
  public:
	/**
	 * Constructor.
	 * @param The next stage in the pipeline.
	 * @return A newly allocated EX object.
	 */
	EX(Stage *next);

	InstrDTO *advance(Response p) override;

  private:
	/**
	 * Sets the (over|under)flow condition code if adding `a` and `b` results in
	 * either.
	 * @param the first operand
	 * @param the second operand
	 */
	void overflow_guard(signed int a, signed int b);
	/**
	 * Maps each mnemonic to a function which carries out the instruction's base
	 * logic.
	 */
	std::unordered_map<
		Mnemonic,
		std::function<void(signed int &s1, signed int &s2)>>
		instr_map;
};

#endif /* EX_H_INCLUDED */
