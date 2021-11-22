#include "cache_simulator.h"

void	cache_hit(Cache *cc, int block_index)
{
	cache_refresh_block(cc, block_index);
	cc->hit_rate++;
}
