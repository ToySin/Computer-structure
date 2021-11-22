#include "cache_simulator.h"

void	rate_print()
{
	unsigned int	total_acess;

	total_acess = miss_rate + hit_rate;

	printf("Acess : %d\n", total_acess);
	printf("Miss rate : %d\n", miss_rate);
	printf("Miss ratio : %f\n", (float)miss_rate / total_acess);
}
