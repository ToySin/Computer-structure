#include "cache_simulator.h"

void	file_read()
{
	char	file_path[40] = "./trace_file/";
	char	str_file[20];

	printf("File : ");
	scanf("%s", str_file);
	strcat(file_path, str_file);

	fp = fopen(file_path, "r");
	if (!fp)
	{
		printf("File open error\n");
		exit(1);
	}
}
