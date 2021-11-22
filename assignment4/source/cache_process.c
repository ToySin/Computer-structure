#include "cache_simulator.h"

void	cache_process(Cache *cc)
{
	block_offset = fetched_inst % cc->block_size;
	cache_index = fetched_inst / cc->block_size % cc->blocks;
	address_tag = fetched_inst / cc->block_size / cc->blocks;

	if (cache_is_hit(cc))
		cache_hit(cc);
	else if (cache_is_miss(cc))
		cache_miss(cc);
}
