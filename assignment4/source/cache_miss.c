#include "cache_simulator.h"

void	cache_miss(Cache *cc)
{
	if (cc->Valid[cache_index] < cc->associate)
	{
		cc->Tag[cache_index][cc->Valid[cache_index]] = address_tag;
		cache_refresh_block(cc, cc->Valid[cache_index]);
		cc->Valid[cache_index]++;
	}
	else if (cc->Valid[cache_index] == cc->associate)
	{
		cc->Tag[cache_index][cc->associate - 1] = address_tag;
		cache_refresh_block(cc, cc->associate - 1);
	}
	cc->miss_rate++;
}
