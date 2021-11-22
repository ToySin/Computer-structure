#include "cache_simulator.h"

void	cache_rate_print(Cache *cc, int type)
{
	unsigned int	total_acess;
	char			cache_type[15];

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
	total_acess = cc->miss_rate + cc->hit_rate;
	printf("\n");
	printf("%s cache\n", cache_type);
	printf("	     Acess : %6d\n", total_acess);
	printf("	 Miss rate : %6d\n", cc->miss_rate);
	printf("	Miss ratio : %.4f\n", (float)cc->miss_rate / total_acess);
	printf("\n");
}
