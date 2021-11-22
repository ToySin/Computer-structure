#include "cache_simulator.h"

void	cache_process(Cache *cc)
{
	if (cache_is_hit(cc))
		cache_hit(cc);
	else if (cache_is_miss(cc))
		cache_miss(cc);
}
