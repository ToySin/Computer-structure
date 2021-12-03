#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

//16진수 2진수 변환함수 정의.
void Convert16to2(char *Hex_array);



FILE *fp;
char *Hex_array; // 파일서 읽어온 16진수를 문자열로 읽기 위한 포인터 변수
unsigned int fetched_inst; //명령어의 16진수를 정수형으로 읽기 위한 변수

////////////////////////////////////////////////////////////////
int operand; // (0일때 read data, 1 write data)  <- data cache , 2 instruction fetch <- instruction cache
char binary_address[33];//16 -> 2진수로 변환 저장 공간

int Access=0, total = 0;
int i_miss=0, w_miss=0, r_miss=0, Miss=0; // Miss = i_miss + w_miss + r_miss
char **cache_data_array, **tag_array;
char *binary_str_tag, *binary_str_index, *binary_str_block_offset;
unsigned long data_index, block_offset, num;
int cache_size, tag_size, index_size, block_offset_size=3, byte_offset_size=2, offset_size=5;
int Hit;

///////////////////////////////////////////////////////////////


void init_data() {
    // cache size 입력 받기
    printf("Unified cache size (KB) : ");
    scanf("%d", &cache_size);

    binary_str_block_offset = (char *)malloc(sizeof(char)*(block_offset_size + 1)); //
    index_size = log2(cache_size);
    binary_str_index = (char *)malloc(sizeof(char)*(index_size + 1));

    tag_size = 32-5-index_size;
	binary_str_tag = (char *)malloc(sizeof(char)*(tag_size + 1));

    // cache_data init
	cache_data_array = (char **)malloc(sizeof(char *)*cache_size);
	for (int i=0;i<index_size;i++) cache_data_array[i] = (char *)malloc(sizeof(char)*(256 + 1)); // (32bits/word * 8)/block

	// tag_array init
	tag_array = (char **)malloc(sizeof(char *)*cache_size);
    for (int i=0;i<index_size;i++) tag_array[i] = (char *)malloc(sizeof(char)*(tag_size + 1 + 1)); // tag + valid bits
}

// int find_index() {// data_index : binary_address 에서 index영역을 10진수 숫자로 저장
//     int return_value=0;
//     //printf("index_size is : %d \n", index_size);
//     for (int i=0; i<index_size;i++) {
//         //printf("binary_address[tag_size+i] is : %c\n", binary_address[tag_size+i]);
//         return_value = (return_value << 1) + binary_address[tag_size+i] - '0';
//     }
//     //printf("result data_index : %d\n", return_value); //성공
//     return return_value;
// }

// int find_block_offset() { // block_offset : binary_address 에서 block offset 영역을 10진수 숫자로 반환
//     int return_value = 0;
//     for (int i=0; i<3;i++) {
//         //printf("binary_address[tag_size+i] is : %c\n", binary_address[tag_size+i]);
//         return_value = (return_value << 1) + binary_address[tag_size+index_size+i] - '0';
//     }
//     printf("block offset is : %d\n", return_value);
//     return return_value;
// }

int main()
{

	char str_file[20] = { 0 };

	// Trace file 입력 받기
	printf("File : ");
	scanf("%s", str_file);

    init_data();

    //  read trace file
	if((fp=fopen(str_file, "r")) == NULL) {
		printf("file open error");
		exit(1);
	}

    //

	Hex_array = (char *)malloc(sizeof(char) * 11);

	while(!feof(fp))		//파일 끝까지 문자열 불러오기.
	{
		fscanf(fp, "%d", &operand);
		fscanf(fp, "%s", Hex_array); //명령어를 문자열로 읽음

        total++; // 접근 횟수 1 증가

        // Hex_array는 16진수 8비트
        // c7e33048  :  1100 0111 1110 0011 0011 0000 0100 1000 <- 32bits binary
        /* Trace file 명령어를 정수형으로 읽기 - 명령어를 문자열로 읽는 함수의 대체로 활용 가능
        fscanf(fp, "%x", &fetched_inst);
        */

		Convert16to2(Hex_array); //16진수 문자열을 2진수 형식의 문자열로 변경
        //정수형으로 명령어를 읽었다면 Convert16to2 함수의 활용은 필요 없음

        //정수형 명령어의 특정 bit 자르기 예시 (tag 22bit, index 8bit, direct-mapped cache)
        //index = (fetched_inst<<22);
        //index = (index>>24); -> index에 해당하는 값이 저장됨

        // index(2진수), tag, offset 얻기
        //memcpy(binary_str_index, binary_address+tag_size, index_size);
        for (int i=0;i<index_size;i++) binary_str_index[i] = binary_address[tag_size+i];
        //printf("binary_str_index : %s\n", binary_str_index);

        //memcpy(binary_str_tag, binary_address, tag_size);
        for (int i=0;i<tag_size;i++) binary_str_tag[i] = binary_address[i];
        //printf("binary_str_tag : %s\n", binary_str_tag);

        //memcpy(binary_str_block_offset, binary_address+tag_size+index_size, block_offset_size);
        for (int i=0;i<block_offset_size;i++) binary_str_block_offset[i] = binary_address[tag_size+index_size+i];
        //printf("binary_str_block_offset : %s\n", binary_str_block_offset);

        data_index = 0, block_offset = 0;
        for (int i=0;i<index_size;i++) data_index = data_index*2 + (binary_str_index[i]-'0');
        for (int i=0;i<3;i++) block_offset = block_offset*2 + (binary_str_block_offset[i]-'0');
        //printf("data_index : %lu\n", data_index);
        //printf("block_offset : %lu\n", block_offset);


        if (tag_array[data_index][0] == '0') { // 유효비트 0 이면
            Miss++; // 미스 증가
            tag_array[data_index][0] = '1'; // 유효비트 바꿔주기
            for  (int i=0;i<tag_size;i++) { // tag배열에 tag 추가
                tag_array[data_index][1+i] = binary_str_tag[i];
            }
        }

        else { // 유효비트가 1이면
            // 태그와 비교 후 다르면 Miss
            // 태그와 같으면 Hit
            // strcmp가 왜 안되지?
            Hit = 1;
            for (int i=0;i<tag_size;i++) {
                if(tag_array[data_index][i+1] != binary_str_tag[i]) {
                    Hit = 0;
                }
            }

            if (Hit == 1) Access++;
            if (Hit != 1) { // Miss이면(Hit가 아니면)
                Miss++;;
                for  (int i=0;i<tag_size;i++) { // tag배열에 tag 추가
                    tag_array[data_index][1+i] = binary_str_tag[i];
                }
            }

        }

	}

    float ratio = (float)Miss/(float)total; //miss rate 계산

    printf("Access : %d\n",Access);
    printf("Miss : %d\n", Miss);
    printf("Miss ratio : %.4f\n", ratio);

	return 0;
}


//16진수 2진수 변환함수.
void Convert16to2(char *Hex_array) // Hex_array -> binary_address
{
    int res=0;
    char temp;
    unsigned long Hex = strtoul(Hex_array, NULL, 16);

    for (int i = 0; i < 32; i++)
    {
        res = Hex & (0x01 << (31 - i));
        if (res == 0)
        {
            temp = '0';
            binary_address[i] = temp;
        }
        else
        {
            temp = '1';
            binary_address[i] = temp;
        }
    }
}
