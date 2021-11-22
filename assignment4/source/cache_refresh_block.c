#include "cache_simulator.h"

void	cache_refresh_block(Cache *cc, int b_idx)
{
	unsigned int	*table;
	unsigned int	refreshing_tag;

	table = cc->Tag[cache_index];
	refreshing_tag = table[b_idx];
	memmove(table + 1, table, sizeof(unsigned int) * b_idx);
	// for (int i = b_idx; i > 0; i--)
	// 	table[i] = table[i - 1];
	table[0] = refreshing_tag;
}
