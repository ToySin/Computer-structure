/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   cache_simulator_challenge.c                                              */
/*                                                                            */
/*   By: 18011562 신동빈                                                      */
/*                                                                            */
/*   DineroIV와 동일하게 작동하는 프로그램                                     */
/*     실제로 Cache는 Valid bit + Tag bit + Data bit(32bit)으로 구성되지만,    */
/*     Cache에 대한 Access와 Miss, Miss ratio만 구하면 되는 프로그램이므로     */
/*     Data bit(32bit)에 대한 구현은 생략하였습니다.                           */
/*                                                                            */
/*   출력 포맷                                                                */
/*     Miss ratio는 Dinero에서 소숫점 4자리까지 보여줍니다.                    */
/*     cache_simulator_challenge.c는 동일하게 4자리까지 보여줍니다.            */
/*                                                                            */
/*   16진수 명령어 처리 방식                                                   */
/*     %x서식자를 이용하여 16진수를 10진수로 읽는다.                            */
/*     shift연산 시 특정 값에서 2를 밑으로 하는 지수값을 알아야 하지만,         */
/*     나눗셈과 모듈로 연산을 이용하면 지수값을 알 필요 없이                    */
/*     바로 수행할 수 있으므로, 나눗셈과 모듈로 연산을 통해서 구현하였습니다.    */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/**
 * @brief Define Cache Type
 * Cache type을 정의한다.
 * data cache -> DATA_TYPE 0
 * instruction cache -> INST_TYPE 1
 * unified cache -> UNIF_TYPE 2
 */
#define DATA_TYPE 0
#define INST_TYPE 1
#define UNIF_TYPE 2


/**
 * @brief File Pointer
 * file을 읽기위한 file pointer
 */
FILE 			*fp;


/**
 * @brief Instruction Operand
 * 명령어의 operand를 저장하기위한 변수
 * 0 read data, 1 write data -> data cache
 * 2 instruction fetch -> instruction cache
 */
int				operand;


/**
 * @brief Instruction fetch
 * 명렁어의 16진수를 정수형으로 저장하기위한 변수
 */
unsigned int	fetched_inst;


/**
 * @brief Tag Binary
 * 명령어에서 tag부분에 해당되는 binary를 정수형으로 저장하기 위한 변수
 */
unsigned int	address_tag;


/**
 * @brief Index Binary
 * 명령어에서 index부분에 해당되는 binary를 정수형으로 저장하기 위한 변수
 */
unsigned int	cache_index;


/**
 * @brief Offset Binary
 * 명령어에서 offset부분에 해당되는 binary를 정수형으로 저장하기 위한 변수
 */
unsigned int	block_offset;


/**
 * @brief Cache
 * Cache 정보를 담는 구조체
 * size -> Cache 크기
 * block_size -> Cache block 크기
 * associate -> Cache set 개수
 * blocks -> Cache entries 개수
 *
 * hit_rate -> Cache hit의 count 수
 * miss_rate -> Cache miss의 count 수
 *
 * Valid -> 명령어의 index에 해당하는 cache block의 유효성을 저장
 * Tag -> 명령어의 tag에 해당하는 binary를 cache block에 저장
 */
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


/**
 * @brief cache_init
 * Cache와 Cache type을 인자로 전달받아, Cache를 초기화한다.
 * Cache size, Cache block size, Cache associativity를 사용자로부터 입력받는다.
 * 입력받은 정보를 바탕으로 Cache entries의 개수를 결정한다.
 * Valid와 Tag를 동적할당하고, *Tag를 associate 만큼 동적할당한다.
 * Valid를 0으로 초기화한다.
 * Cache hit/miss rate를 0으로 초기화한다.
 * @param cc
 * @param type
 */
void	cache_init(Cache *cc, int type);


/**
 * @brief cache_free
 * Cache 구조체에서 동적할당 된 메모리를 모두 반환한다.
 * @param cc
 */
void	cache_free(Cache *cc);


/**
 * @brief cache_match_block_index
 * 현재 명령어의 index에 해당하는 cache entry에서
 * 현재 명령어의 tag에 해당하는 block이 있는지 탐색한다.
 * 있을 시, 그 tag가 저장된 block의 인덱스(LRU 우선순위)
 * 없을 시, (-1)을 리턴한다.
 * @param cc
 * @return int
 */
int		cache_match_block_index(Cache *cc);


/**
 * @brief cache_process
 * 현재 명령어로 cache process를 수행한다.
 * Cache 정보(cache size, cache block size, cache associativity)
 * Cache 정보를 이용해서 현재 명령어를 tag, index, offset으로 나눈다.
 * 명령어 정보를 이용해서 cache hit/miss를 판단한다.
 * hit/miss에 따른 process를 수행한다.
 * @param cc
 */
void	cache_process(Cache *cc);


/**
 * @brief cache_hit
 * Cache hit의 경우에 현재 명령어에 해당하는
 * Cache block을 refresh한다. (LRU 우선순위 refresh)
 * Cache hit_rate를 증가시킨다.
 * @param cc
 * @param block_index
 */
void	cache_hit(Cache *cc, int block_index);


/**
 * @brief cache_miss
 * Cache miss의 경우에 현재 명령어에 해당하는
 * index위치에 현재 tag를 저장한다.
 * @param cc
 */
void	cache_miss(Cache *cc);


/**
 * @brief cache_refresh_block
 * 최근에 접근 된 Cache block에 대해서 LRU우선순위를 갱신한다.
 * @param cc
 * @param b_idx
 */
void	cache_refresh_block(Cache *cc, int b_idx);


/**
 * @brief cache_rate_print
 * Cache의 hit/miss정보를 출력한다.
 * 출력내용:
 * 		Access		-> Cache에 대한 전체 접근 횟수
 * 		Miss		-> Cache miss의 횟수
 * 		Miss ratio	-> 전체 접근에 대한 Cache miss의 비율
 * @param cc
 * @param type
 */
void	cache_rate_print(Cache *cc, int type);


/**
 * @brief file_read
 * 사용자로부터 파일명을 입력받아 해당 파일을 오픈한다.
 * 성공 시 fp에 저장되며, 실패 시 전체 프로세스가 종료된다.
 */
void	file_read();


/**
 * @brief file_read_line
 * 현재 open되어있는 파일에서 한 줄을 읽어온다.
 * operand를 먼저 읽고, 16진수 형태의 명령어를 정수형으로 fetched_inst에 저장한다.
 */
void	file_read_line();


int	main()
{
	Cache data_cache;
	Cache inst_cache;

	/**
	 * @brief file open
	 * 사용자로부터 파일명을 입력받아서 파일을 오픈한다.
	 * trace_1.din or trace_2.din
	 */
	file_read();

	/**
	 * @brief cache init
	 * 사용자로부터 cache정보를 입력받아서 cache를 초기화한다.
	 * cache size(KB), cache block size(B), cache associativity
	 */
	cache_init(&inst_cache, INST_TYPE);
	cache_init(&data_cache, DATA_TYPE);

	while (!feof(fp))
	{
		/**
		 * @brief read line from file
		 * get operand and fetched_inst(0x -> uint)
		 */
		file_read_line();

		/**
		 * @brief data cache process
		 * operand == 0 -> read data
		 * operand == 1 -> write data
		 */
		if(operand == 0 || operand == 1)
		{
			cache_process(&data_cache);
		}

		/**
		 * @brief instruction cache process
		 * operand == 2 -> instruction fetch
		 */
		if(operand == 2)
		{
			cache_process(&inst_cache);
		}
	}

	/**
	 * @brief print cache info
	 * Access, Miss, Miss ratio
	 */
	cache_rate_print(&inst_cache, INST_TYPE);
	cache_rate_print(&data_cache, DATA_TYPE);

	/**
	 * @brief free cache memory
	 * Valid, Tag
	 */
	cache_free(&data_cache);
	cache_free(&inst_cache);
	return (0);
}


void	cache_init(Cache *cc, int type)
{
	char	cache_type[15];

	/**
	 * @brief cache type
	 * cache type에 따라서 출력 명령이 달라진다.
	 */
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

	/**
	 * @brief user input
	 * 사용자로부터 cache 정보를 입력받는다.
	 */
	printf("%s Cache Size (KB) : ", cache_type);
	scanf("%d", &cc->size);
	printf("%s Cache Blcok Size (B) : ", cache_type);
	scanf("%d", &cc->block_size);
	printf("%s Cache Associativity : ", cache_type);
	scanf("%d", &cc->associate);

	/**
	 * @brief cache blocks
	 * 사용자로부터 입력받은 정보를 이용하여 cache entries의 크기를 구한다.
	 * 1024 = pow(2, 10) | cache size(KB) / cache block size(B) -> n * (K)
	 * (K) = 1024
	 * 사용자로부터 입력받은 정보에는 단위가 포함되지 않는다.
	 * 따라서 직접 단위를 변환해주는 작업을 수행한다.
	 */
	cc->blocks = cc->size / cc->block_size * 1024 / cc->associate;

	/**
	 * @brief allocate memory
	 * cache entries에 따라 Valid, Tag를 동적할당한다.
	 */
	cc->Valid = (int *)malloc(sizeof(int) * cc->blocks);
	cc->Tag = (unsigned int **)malloc(sizeof(unsigned int *) * cc->blocks);
	if (!cc->Valid || !cc->Tag)
	{
		printf("Memory is not allocated!\n");
		exit(1);
	}

	/**
	 * @brief initial Valid, Tag
	 * cache entries 각각에 해당하는 Valid는 0으로 초기화한다.
	 * entry마다 Tag는 associativity만큼 동적할당 한다.
	 */
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

	/**
	 * @brief
	 * cache의 hit/miss 횟수를 0으로 초기화한다.
	 */
	cc->hit_rate = 0;
	cc->miss_rate = 0;
}


void	cache_free(Cache *cc)
{
	free(cc->Valid);
	for (int i = 0; i < cc->blocks; i++)
		free(cc->Tag[i]);
	free(cc->Tag);
}


int	cache_match_block_index(Cache *cc)
{
	/**
	 * @brief search index
	 * 현재 명령어가 저장 될 수 있는 cache entry index에서
	 * 현재 명령어의 tag값이 있는지 확인한다.
	 * hit의 경우 entry에서 tag의 LRU우선순위를 리턴한다.
	 * miss의 경우 for반복문을 통과해서 (-1)을 리턴한다.
	 * @param i
	 */
	for (int i = 0; i < cc->Valid[cache_index]; i++)
	{
		if (cc->Tag[cache_index][i] == address_tag)
			return (i);
	}
	return (-1);
}


void	cache_process(Cache *cc)
{
	int	block_index;

	/**
	 * @brief binary extraction
	 * 현재 명령어를 cache정보를 이용하여
	 * tag, index, offset으로 분할하여 저장한다.
	 */
	block_offset = fetched_inst % cc->block_size;
	cache_index = fetched_inst / cc->block_size % cc->blocks;
	address_tag = fetched_inst / cc->block_size / cc->blocks;

	/**
	 * @brief cache process
	 * hit의 경우 cache_hit process를 실행한다.
	 * miss의 경우 cache_miss process를 실행한다.
	 */
	block_index = cache_match_block_index(cc);
	if (block_index == -1)
	{
		cache_miss(cc);
	}
	else
	{
		cache_hit(cc, block_index);
	}
}


void	cache_hit(Cache *cc, int block_index)
{
	/**
	 * @brief hit process
	 * hit의 경우 현재 접근 한 block의 LRU우선순위를 최상위로 갱신한다.
	 * hit count를 증가한다.
	 */
	cache_refresh_block(cc, block_index);
	cc->hit_rate++;
}


void	cache_miss(Cache *cc)
{
	/**
	 * @brief miss process
	 * miss의 경우에 두가지로 나눠진다.
	 * 1. 현재 entry에 비어있는 associate set이 남아있다.
	 * 		비어있는 associate set에 tag를 저장하고
	 * 		이번에 접근한 block이므로, LRU우선순위를 최상위로 갱신한다.
	 * 2. 현재 entry에 associate set이 가득 차있다.
	 * 		LRU우선순위가 가장 낮은 block의 tag를 현재 명령어의 tag로 갱신하고,
	 * 		LRU우선순위를 최상위로 갱신한다.
	 * 공통으로 miss count를 증가한다.
	 */
	if (cc->Valid[cache_index] < cc->associate)
	{
		cc->Tag[cache_index][cc->Valid[cache_index]] = address_tag;
		cache_refresh_block(cc, cc->Valid[cache_index]);
		cc->Valid[cache_index]++;
	}
	else if (cc->Valid[cache_index] == cc->associate)
	{
		cc->Tag[cache_index][cc->associate - 1] = address_tag;
		cache_refresh_block(cc, cc->associate - 1);
	}
	cc->miss_rate++;
}


void	cache_refresh_block(Cache *cc, int b_idx)
{
	unsigned int	*table;
	unsigned int	refreshing_tag;

	/**
	 * @brief current entry block table
	 * table 변수가 현재 index에 따른 cache block entry를 가르키게 한다.
	 * refreshing_tag는 LRU우선순위에서 최상위 우선순위가 될 tag정보를 저장한다.
	 * LRU우선순위를 갱신하는 방법은, array형에서 index 0에 위치하도록 한다.
	 * 기존의 tag정보를 메모리 단위에서 (4Byte)씩 밀어낸다.
	 * memmove를 이용해서 (unsigned int)->4Byte * 갱신 할 블럭 인덱스 만큼을
	 * table + 1위치로 밀어낸다.
	 * 이후 table[0]위치에 refreshing_tag를 저장한다.
	 */
	table = cc->Tag[cache_index];
	refreshing_tag = table[b_idx];
	memmove(table + 1, table, sizeof(unsigned int) * b_idx);
	table[0] = refreshing_tag;
}


void	cache_rate_print(Cache *cc, int type)
{
	unsigned int	total_access;
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

	/**
	 * @brief total access
	 * 전체 접근 횟수는 miss_rate + hit_rate
	 */
	total_access = cc->miss_rate + cc->hit_rate;
	printf("\n");
	printf("%s cache\n", cache_type);
	printf("	    Access : %6d\n", total_access);
	printf("	 Miss rate : %6d\n", cc->miss_rate);
	printf("	Miss ratio : %.4f\n", (float)cc->miss_rate / total_access);
	printf("\n");
}


void	file_read()
{
	/**
	 * @brief terminal
	 * Linux 환경에서 trace_file/trace_1.din, trace_file/trace_2.din
	 * 디렉토리 구조로 관리했으나, 터미널마다 파일경로 표현이 다르다.
	 * cache_simulator_challenge.c 소스파일과 같은 디렉토리 경로에 있는
	 * trace_1.din, trace_2.din을 읽어오도록 수정했다.
	 */
	char	str_file[20];

	printf("File : ");
	scanf("%s", str_file);

	fp = fopen(str_file, "r");
	if (!fp)
	{
		printf("File open error\n");
		exit(1);
	}
}


void	file_read_line()
{
	fscanf(fp, "%d", &operand);
	fscanf(fp, "%x", &fetched_inst);
}
