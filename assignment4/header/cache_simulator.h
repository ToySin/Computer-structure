#ifndef CACHE_SIMULATOR_H
# define CACHE_SIMULATOR_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <stdbool.h>

# define DATA_TYPE 0
# define INST_TYPE 1
# define UNIF_TYPE 2

///////////////////////////////////////////////////////////////
FILE 			*fp;
////////////////////////////////////////////////////////////////
int				operand; // (0일때 read data, 1 write data)  <- data cache , 2 instruction fetch <- instruction cache
unsigned int	fetched_inst; //명령어의 16진수를 정수형으로 읽기 위한 변수
///////////////////////////////////////////////////////////////
unsigned int	address_tag;
unsigned int	cache_index;
unsigned int	block_offset;
///////////////////////////////////////////////////////////////

typedef struct Cache
{
	unsigned int	size;
	unsigned int	block_size;
	unsigned int	associate;
	unsigned int	blocks;

	unsigned int	hit_rate;
	unsigned int	miss_rate;

	unsigned int	*Valid;
	unsigned int	**Tag;
}					Cache;

int		cache_match_block_index(Cache *cc);
void	cache_init(Cache *cc, int type);
void	cache_free(Cache *cc);
void	cache_process(Cache *cc);
void	cache_hit(Cache *cc, int block_index);
void	cache_miss(Cache *cc);

void	cache_refresh_block(Cache *cc, int b_idx);

void	cache_rate_print(Cache *cc, int type);

void	file_read();
void	file_read_line();

#endif
