#include "cache_simulator.h"

bool	cache_is_miss(Cache *cc)
{
	return (!cc->Valid[cache_index] ||
			(cc->Tag[cache_index] != address_tag));
}
