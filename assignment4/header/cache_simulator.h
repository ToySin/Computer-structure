#ifndef CACHE_SIMULATOR_H
# define CACHE_SIMULATOR_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <stdbool.h>

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
int				hit_rate;
int				miss_rate;
///////////////////////////////////////////////////////////////

typedef struct Cache
{
	unsigned int	size;
	unsigned int	block_size;
	unsigned int	blocks;

	bool			*Valid;
	unsigned int	*Tag;
}					Cache;

bool	cache_is_miss(Cache *cc);
bool	cache_is_hit(Cache *cc);

void	cache_init(Cache *cc);
void	cache_free(Cache *cc);

void	cache_process(Cache *cc);
void	cache_hit(Cache *cc);
void	cache_miss(Cache *cc);


void	file_read();
void	file_read_line(Cache *cc);


void	rate_init();
void	rate_print();

#endif
