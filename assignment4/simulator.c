#include "cache_simulator.h"

int	main()
{
	Cache data_cache;
	Cache inst_cache;

	file_read();
	cache_init(&inst_cache, INST_TYPE);
	cache_init(&data_cache, DATA_TYPE);

	while (!feof(fp))
	{
		file_read_line();

		if(operand == 0 || operand == 1)	//data cahce 동작 구현
		{
			cache_process(&data_cache);
		}
		if(operand == 2)					//inst cache 동작 구현
		{
			cache_process(&inst_cache);
		}
	}
	cache_rate_print(&inst_cache, INST_TYPE);
	cache_rate_print(&data_cache, DATA_TYPE);

	cache_free(&data_cache);
	cache_free(&inst_cache);
	return (0);
}


