#include "cache_simulator.h"

void	cache_miss(Cache *cc)
{
	cc->miss_rate++;
	cc->Tag[cache_index] = address_tag;
	cc->Valid[cache_index] = true;
}
