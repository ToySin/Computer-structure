#include "cache_simulator.h"

void	file_read_line()
{
	fscanf(fp, "%d", &operand);
	fscanf(fp, "%x", &fetched_inst);
}
