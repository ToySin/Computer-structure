#include "cache_simulator.h"

void	cache_free(Cache *cc)
{
	free(cc->Valid);
	for (int i = 0; i < cc->blocks; i++)
		free(cc->Tag[i]);
	free(cc->Tag);
}
