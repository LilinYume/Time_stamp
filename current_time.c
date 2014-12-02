#include "current_time.h"

/* 関数呼び出し時の今日の日付
	フォーマット 例：2001/04/01
	必要：引数へはサイズDATE_LEN分が割り当てられた配列を渡す
-------------------------------------------------------------------------- */
void today(char* dt)
{
	time_t date = 0;
	date = time(NULL);

	strftime(dt, DATE_LEN, "%Y/%m/%d", localtime(&date));
}
/* 関数呼び出し時の現在時刻
	フォーマット 例：13:05:40
	必要：引数へはサイズTIME_LEN分が割り当てられた配列を渡す
------------------------------------------------------------------------- */
void now(char* tv)
{
	time_t date = 0;
	date = time(NULL);

	strftime(tv, TIME_LEN, "%H:%M:%S", localtime(&date));
}
