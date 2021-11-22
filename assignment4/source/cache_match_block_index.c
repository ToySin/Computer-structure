#include "cache_simulator.h"

int	cache_match_block_index(Cache *cc)
{
	for (int i = 0; i < cc->Valid[cache_index]; i++)
	{
		if (cc->Tag[cache_index][i] == address_tag)
			return (i);
	}
	return (-1);
}
