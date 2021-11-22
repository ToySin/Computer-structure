#include "cache_simulator.h"

void	cache_process(Cache *cc)
{
	int	block_index;


	block_offset = fetched_inst % cc->block_size;
	cache_index = fetched_inst / cc->block_size % cc->blocks;
	address_tag = fetched_inst / cc->block_size / cc->blocks;

	block_index = cache_match_block_index(cc);
	if (block_index == -1)
	{
		cache_miss(cc);
	}
	else
	{
		cache_hit(cc, block_index);
	}
	// printf("%d", cache_index);
	// for (int i = 0; i < cc->Valid[cache_index]; i++)
	// 	printf("  %d  ->", cc->Tag[cache_index][i]);
	// printf("\n");
}
