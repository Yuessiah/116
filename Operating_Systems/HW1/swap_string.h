static char* swap_string_solve(const char* in)
{
	int flg;
	char str[100], tmp[100];
	char* out;

	sscanf(in, "%d%s", &flg, str);
	strcpy(tmp, str);
	tmp[flg] = '\n'; tmp[flg+1] = '\0';
	strcpy(str+strlen(str), tmp);

	return out = str+flg;
}
