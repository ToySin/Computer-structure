#include "cache_simulator.h"

void	cache_init(Cache *cc, int type)
{
	char	cache_type[15];

	if (type == DATA_TYPE)
		strcpy(cache_type, "Data");
	else if (type == INST_TYPE)
		strcpy(cache_type, "Instruction");
	else if (type == UNIF_TYPE)
		strcpy(cache_type, "Unified");
	else
	{
		printf("Wrong type number\n");
		exit(1);
	}
	printf("%s Cache Size (KB) : ", cache_type);
	scanf("%d", &cc->size);
	printf("%s Cache Blcok Size (B) : ", cache_type);
	scanf("%d", &cc->block_size);
	printf("%s Cache Associativity : ", cache_type);
	scanf("%d", &cc->associate);

	cc->blocks = cc->size / cc->block_size * 1024 / cc->associate;

	cc->Valid = (int *)malloc(sizeof(int) * cc->blocks);
	cc->Tag = (unsigned int **)malloc(sizeof(unsigned int *) * cc->blocks);
	if (!cc->Valid || !cc->Tag)
	{
		printf("Memory is not allocated!\n");
		exit(1);
	}

	for (int i = 0; i < cc->blocks; i++)
	{
		cc->Valid[i] = 0;
		cc->Tag[i] = (unsigned int *)malloc(sizeof(unsigned int) * cc->associate);
		if (!cc->Tag[i])
		{
			printf("Memory is not allocated!\n");
			exit(1);
		}
	}
	cc->hit_rate = 0;
	cc->miss_rate = 0;
}
