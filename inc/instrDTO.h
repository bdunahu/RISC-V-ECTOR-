#ifndef INSTRDTO_H
#define INSTRDTO_H
#include <string>
#include <functional>

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
	 * @return id_cycle
	 */
	int get_id_cycle();
	/**
	 * @return instr_bits
	 */
	signed int get_instr_bits();
	/**
	 * @return s1
	 */
	signed int get_s1();
	/**
	 * @return s2
	 */
	signed int get_s2();
	/**
	 * @return s3
	 */
	signed int get_s3();
	/**
	 * @return the string representation of oper.
	 */
	std::string get_oper_name();

	/**
	 * @param if_cycle
	 */
	void set_if_cycle(int);
	/**
	 * @param id_cycle
	 */
	void set_id_cycle(int);
	/**
	 * @param instr_bits
	 */
	void set_instr_bits(signed int);
	/**
	 * @param s1
	 */
	void set_s1(signed int);
	/**
	 * @param s2
	 */
	void set_s2(signed int);
	/**
	 * @param s3
	 */
	void set_s3(signed int);

  private:
	/**
	 * The clock cycle this instruction finished being fetched.
	 */
	int if_cycle;
	/**
	 * The clock cycle this instruction finished being identified.
	 */
	int id_cycle;
	/**
	 * The raw bits encoding the instruction.
	 */
	signed int instr_bits;
	/**
	 * Slots in this instruction, for storing temporary registers, immediates,
	 * or other.
	 * Some instruction types may use these differently.
	 * The `oper` function is in charge of knowing how to parse these.
	 */
	signed int s1;
	signed int s2;
	signed int s3;
  /**
   * The operation to be conducted during the execute phase.
   */
	std::function<void()> instr;
};

#endif /* INSTRDTO_H_INCLUDED */
