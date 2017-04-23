#include<stdio.h>
#include<math.h>

int main()
{
	int n1, n2;
	printf("layers: ");
	scanf("%d", &n1);
	printf("child:  ");
	scanf("%d", &n2);

	for(int i = 0; i < n1; i++) {
		for(int j = 1; j <= (int)pow(n2, i); j++) {
			if(j != 1 && i < n1-1) for(int k = 1; k <= (int)pow(n2, n1-1)/(int)pow(n2, i) - 1; k++) putchar(' ');
			for(int k = 1; k <= (int)pow(n2, n1-1)/(int)pow(n2, i); k++) putchar(' ');
			putchar('o');
		}
		printf("\n\n");
	}

	return 0;
}
