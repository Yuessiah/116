#include<stdio.h>

int Recursive(int n)
{
	if(n == 1) return 1;
	return Recursive(n-1)*n;
}

int Iterative(int n)
{
	int ans = 1;
	for(int i = n; i >= 1; i--) ans *= i;
	return ans;
}

int main()
{
	int n;
	while(scanf("%d", &n)) {
		if(n == -1) break;
		printf("Recursive: %d\n", Recursive(n));
		printf("Iterative: %d\n", Iterative(n));
	}

	return 0;
}
