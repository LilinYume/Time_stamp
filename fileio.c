#include "fileio.h"

/* ファイルの存在チェック 
	通常ファイルではない場合も存在しないと見なす。
	存在する場合1を返し、存在しない場合は0（ゼロ）を返す
------------------------------------------------------------------------- */
int file_exist(const char* path)
{
	struct stat finfo;
	if(stat(path, &finfo) == -1 && !S_ISREG(finfo.st_mode)) {
		perror("stat: \n");
		return 0;
	}
	return 1;
}
/* ファイルへ日付、時刻を書き込む。
	書き込み方（ファイル・フォーマット）：未定
------------------------------------------------------------------------- */
void time_stamp(void)
{
	char date[DATE_LEN] = {0};
	char nowtime[TIME_LEN] = {0};

	today(date);
	now(nowtime);

	FILE* writer = NULL; 
	FILE* writelog = NULL; 
	
	if((writer = fopen(recfile.name, "w")) == NULL)
		error("file cannot open\n");
	
 	fprintf(writer,"%s %s", date, nowtime);
	fclose(writer);

	if((writelog = fopen(recfile_log.name, "a")) == NULL)
		error("file cannot open\n");

 	fprintf(writer,"%s %s\n", date, nowtime);
	fclose(writelog);

}
/* ファイルから日付・時刻を読み込む
	読み取りは行単位で行う。
	引数lineへは割り当て済みの配列を渡す。
------------------------------------------------------------------------- */
char* read_line(char* line)
{
	FILE* reader = NULL; 
	if((reader = fopen(recfile.name, "r")) == NULL)
		error("reading error\n");
	
	return fgets(line, MAXLEN, reader);
}
/* 時間の差を表示
	現在の日付・時刻とファイルに記録された時間の差（単位：秒）
	注：実際の時間差の計算と出力はprint_level関数で行う。
------------------------------------------------------------------------- */
void show_diff(void)
{
	char str[MAXLEN] = {0};
	if((read_line(str)) == NULL)
		error("can not read the data from file");

	struct tm rec;

	time_t pass = passed_date(rec, str);
	time_t curr = time(NULL);

	if(curr < pass) error("compared wrong value\n");

	double diff = difftime(curr, pass);
	long diff_sec = diff;
	
	print_level(diff_sec);
}
/* ファイルから取得した年、月、日、時、分、秒（各int）をtime_t型にかえる
------------------------------------------------------------------------- */
time_t passed_date(struct tm old, const char* date)
{
	char buf[DATE_LEN+TIME_LEN+1] = {0};
	int year, month, day;
	int hour, miniture, second;

	strncpy(buf, date, sizeof buf);

	int succ = sscanf(buf, "%4d%*c%2d%*c%2d%*c%2d%*c%2d%*c%2d",
			&year, &month, &day, &hour, &miniture, &second);
	
	if(succ != 6) error("unreadable field\n");
	
	old.tm_year = year-1900;
	old.tm_mon = month-1;
	old.tm_mday = day;
	old.tm_hour = hour;
	old.tm_min = miniture;
	old.tm_sec = second;
	
 	return mktime(&old);
}
/* 経過時間（秒）をそれぞれ、分、時、日に変えて出力
	経過時間（秒）はshow_diff関数から渡される
	月単位はなし
------------------------------------------------------------------------- */
void print_level(long diffsec)
{
	if(diffsec < SECONDS) {
		printf("%ld sec\n", diffsec);
		return;
	}
	long min = diffsec / SECONDS;
	long sec = diffsec % SECONDS;

	if(min < ONE_HOUR) {
		printf("%ld min, %ld sec\n", min, sec);
		return;
	}
	long hour = min / ONE_HOUR;
	min %= ONE_HOUR;

	if(hour < ONE_DAY) {
		printf("%ld hour, %ld min\n", hour, min);
		return;
	}
	long day = hour / ONE_DAY;
	hour %= ONE_DAY;

	if(day < ONE_YEAR) {
		printf("%ld day, %ld hour\n", day, hour);
		return;
	}
	long year = day / ONE_YEAR;
	day %= ONE_YEAR;
	printf("%ld year, %ld day\n", year, day);
}
