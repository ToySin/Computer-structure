#include "cache_simulator.h"

void	cache_free(Cache *cc)
{
	free(cc->Valid);
	free(cc->Tag);
}
