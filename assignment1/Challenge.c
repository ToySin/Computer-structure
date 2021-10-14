#include <stdio.h>

int	myFibo(int n, int a)
{
	int t0 = 1;
	int t1 = 1;
	printf("1   1   ");
	int i = 1;
	do
	{
		t0 = t0 + t1 + a;
		printf("%d   ", t0);
		int temp = t0;
		t0 = t1;
		t1 = temp;
		i++;
	} while (i < n);
}

void	isMultofThree(int n)
{
	if (n % 3 < 0)
		printf("%d is mult~~\n", n);
	else
		printf("%d is not mult~~\n", n);
}

int	main()
{
	int n, a;
	scanf("%d %d", &n, &a);
	myFibo(n, a);
}