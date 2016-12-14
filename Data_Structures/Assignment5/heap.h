#ifndef _HEAP_H_
#define _HEAP_H_

#include<algorithm>
#include<cstdio>

namespace heap {
	int heapList[256], heapSize = 0;

	void percolate(int i)
	{
		while(i>>1) {
			if(heapList[i] > heapList[i>>1])
				std::swap(heapList[i], heapList[i>>1]);
			i >>= 1;
		}
	}

	void insert(int e)
	{
		heapList[++heapSize] = e;
		percolate(heapSize);
	}

	void clear()
	{
		heapSize = 0;
	}

	void output()
	{
		for(int i = 1; i <= heapSize; i<<=1) {
			for(int j = i; j < (i<<1) && j <= heapSize; j++)
				printf("%d ", heapList[j]);
			puts("");
		}
	}
}

#endif
