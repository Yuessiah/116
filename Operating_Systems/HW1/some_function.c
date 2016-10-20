#include"some_function.h"

char* dig_to_str(int dig)
{
	char rsum[10];
	static char psum[10];
	int len = 0, i, c = 0;
	while(dig%10) {
		rsum[len++] = (dig%10)+'0';
		dig /= 10;
	}
	for(i = len-1; i >= 0; i--) psum[c++] = rsum[i];
	psum[c] = '\0';
	return psum;
}
int str_to_dig(char* str, int len)
{
	int sum = 0, pos = 1;
	while(len--) {
		sum += (str[len]-'0')*pos;
		pos *= 10;
	}
	return sum;
}
int is_digit(char c)
{
	int d = c-'0';
	if(d <= 9 && d >= 0) return 1;
	return 0;
}
