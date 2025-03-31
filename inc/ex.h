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
	using Stage::advance;

  private:
	void advance_helper();
	/**
	 * Maps each mnemonic to a function which carries out the instruction's base
	 * logic.
	 * All instructions store the result into s1.
	 */
	std::unordered_map<
		Mnemonic,
		std::function<void(signed int &s1, signed int s2, signed int s3)>>
		instr_map;
};

#endif /* EX_H_INCLUDED */
