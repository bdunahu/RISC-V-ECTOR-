#ifndef INSTRDTO_H
#define INSTRDTO_H

class InstrDTO
{
  public:
	/**
	 * Constructor.
	 */
	InstrDTO();
	~InstrDTO() = default;

	/**
	 * @return if_cycle
	 */
	int get_if_cycle();
	/**
	 * @return instr_bits
	 */
	signed int get_instr_bits();

	/**
	 * @param if_cycle
	 */
	void set_if_cycle(int);
	/**
	 * @param instr_bits
	 */
	void set_instr_bits(signed int);

  private:
	/**
	 * The current clock cycle.
	 */
	int if_cycle;
	signed int instr_bits;
};

#endif /* INSTRDTO_H_INCLUDED */
