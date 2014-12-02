#include "util.h"

/* ユーティリティ */
void error(const char* msg)
{
	fprintf(stderr,"%s", msg);
	exit(1);
}
/* ユーティリティ */
#if 0
char get_opt(const char* val)
{
	int len = 0;
	while(val[len++]); 

	int index = 0;
	if(val[index] != '-' || len <= 2) return -1;

	return val[++index];
}
#endif
