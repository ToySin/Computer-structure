#include <stdio.h>
#include <math.h>

int main()
{
	// 1508932017997
	// 10101111101010011010110110101111101001101(2)
	// 0x15f535b5f4d
	unsigned long fetch = 1508932017997;

	// offset = fetch % pow(2, 5) (Basic단계에서는 고정값)
	// 01101 13
	// index = fetch >> 5 % pow(2, m)
	// 1011111010 762
	// tag = fetch >> 5 >> m
	// 10101111101010011010110110 46048950

	printf("%ld ", fetch % (int)pow(2, 5));
	printf("%ld ", (fetch >> 5) % (int)pow(2, 10));
	printf("%ld\n", (fetch >> 5) >> 10);
}
