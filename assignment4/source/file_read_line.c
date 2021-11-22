#include "cache_simulator.h"

void	file_read_line(Cache *cc)
{
	fscanf(fp, "%d", &operand);
	fscanf(fp, "%x", &fetched_inst);

	block_offset = fetched_inst % cc->block_size;
	cache_index = fetched_inst / cc->block_size % cc->blocks;
	address_tag = fetched_inst / cc->block_size / cc->blocks;
}
