#ifndef DRAM_H
#define DRAM_H
#include <storage.h>

class Dram : public Storage
{
  public:
	Dram();
	~Dram();

	int *load_line(int);

  private:
};

#endif /* DRAM_H_INCLUDED */
