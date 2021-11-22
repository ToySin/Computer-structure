#include <stdio.h>
#include <math.h>

int main()
{
	//1398497101
	// 01010011010110110 101111101001101(2)
	// 0x535b5f4d
	unsigned int fetch = 1398497101;

	// offset = fetch % pow(2, 5) (Basic단계에서는 고정값)
	// 01101 13
	// index = fetch >> 5 % pow(2, m)
	// 1011111010 762
	// tag = fetch >> 5 >> m
	//  42678

	printf("%d ", fetch % (int)pow(2, 5));
	printf("%d ", (fetch >> 5) % (int)pow(2, 10));
	printf("%d ", (fetch << 17) >> 22);
	printf("%d\n", (fetch >> 5) >> 10);

	printf("%d ", fetch % 32);
	printf("%d ", fetch / 32 % 1024);
	printf("%d ", (fetch << 17) >> 22);
	printf("%d\n", fetch / 32 / 1024);
}

// 한번에 주석 설정 Ctrl + k and Ctrl + c
// 한번에 주석 해제 Ctrl + k and Ctrl + u

// if(operand == 0 || operand == 1)		//data cahce 동작 구현
// {
// 	printf("data %d    %s    %s\n",operand, Hex_array, binary);
// }
// if(operand == 2)				//inst cache 동작 구현
// {
// 	printf("inst %d    %s    %s\n",operand, Hex_array, binary);
// }
