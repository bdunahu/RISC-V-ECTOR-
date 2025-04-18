#ifndef EX_H
#define EX_H
#include "instrDTO.h"
#include "response.h"
#include "stage.h"
#include <exception>
#include <unordered_map>

class HaltException : public std::exception
{
	const char *what() const noexcept override {
		return "";
	}
};

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
	 * Wrapper for division functions, which detects HALT instructinos (division
	 * by 0).
	 * @param the numerator
	 * @param the denominator
	 * @param if the modulo operator should instead be used
	 */
	void handle_divide(signed int &s1, signed int s2, bool is_mod);
	/**
	 * Maps each mnemonic to a function which carries out the instruction's base
	 * logic.
	 * All instructions store the result into s1.
	 */
	std::unordered_map<
		Mnemonic,
		std::function<void(
			signed int &s1, signed int s2, signed int s3, unsigned int pc)>>
		instr_map;
};

#endif /* EX_H_INCLUDED */
