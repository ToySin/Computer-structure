#include "cache_simulator.h"

int	main()
{
	Cache cache;

	file_read();
	cache_init(&cache);
	rate_init();

	while (!feof(fp))
	{
		file_read_line(&cache);
		cache_process(&cache);
	}
	rate_print();
	cache_free(&cache);
	return (0);
}
