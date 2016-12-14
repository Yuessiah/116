#include<cstdio>
#include<cstdlib>
#include<ctime>
#include"heap.h"
const int maxn = 100;

FILE* fd;
int in[maxn], n;

void input()
{
	fd = fopen("in.txt", "r");

	char data[256];
	fgets(data, 256, fd);
	int cur = 0;
	while(1) {
		in[n++] = atoi(data+cur);
		while(data[cur++] != ',' && data[cur] != '\0');
		if(data[cur] == '\0') break;
	}

	fclose(fd);
}

int main()
{
	n = 0;
	input();

	puts("Create Max Heap :");
	for(int i = 0; i < n; i++) heap::insert(in[i]);

	puts("Max Heap :");
	heap::output();

	while(1) {
		printf("Change priority ? (Y/N) : ");
		char ok[2]; scanf("%s", ok);

		if(ok[0] == 'Y') {
			int a, b;
			printf("Choice priority : ");
			scanf("%d", &a);
			printf("Change priority to : ");
			scanf("%d", &b);

			clock_t start = clock();

			heap::clear();
			for(int i = 0; i < n; i++) if(in[i] == a) in[i] = b;
			for(int i = 0; i < n; i++) heap::insert(in[i]);

			clock_t end = clock();

			puts("Max Heap :");
			heap::output();

			printf("Spend time : %.5f sec\n", (double)(end-start) / CLOCKS_PER_SEC);
		} else break;
	}

	return 0;
}
