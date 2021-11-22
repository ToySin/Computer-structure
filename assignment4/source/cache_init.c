#include "cache_simulator.h"

void	cache_init(Cache *cc)
{
	printf("Unified Cache Size (KB) : ");
	scanf("%d", &cc->size);

	cc->block_size = 32;
	cc->blocks = cc->size / cc->block_size * 1024;

	cc->Valid = (bool *)malloc(sizeof(bool) * cc->blocks);
	cc->Tag = (unsigned int *)malloc(sizeof(unsigned int) * cc->blocks);
	if (!cc->Valid || !cc->Tag)
	{
		printf("Memory is not allocated!\n");
		exit(1);
	}

	for (int i = 0; i < cc->blocks; i++)
		cc->Valid[i] = false;
}
