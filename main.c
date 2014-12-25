#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
/* ------------------------------------------------------------------------- */
#define DATE_LEN 20 /* "yyyy/mm/dd hh:mm:ss" */
#define MAXLEN 100	/* for read_line() */

#define SECONDS 60
#define ONE_MIN 60
#define ONE_HOUR 60
#define ONE_DAY 24
#define ONE_YEAR 365

#define FILENAME_LEN 256
#define MAXLEN 100
/* ------------------------------------------------------------------------- */
struct file_location{ char name[FILENAME_LEN]; } recfile, recfile_log;
/* ------------------------------------------------------------------------- */
void init_file_location(void);
void default_filename(void);
void other_filename(const char* path);
void date_time(char* dt);
int file_exist(const char* path);
void time_stamp(void);
void time_stamp_log(void);
int read_line(char* line,FILE* src);
void show_diff(const char* name);
time_t passed_date(struct tm old, const char* date);
void print_level(long diffsec);
void error(const char* msg);
//char get_opt(const char* val); //not used
/* ------------------------------------------------------------------------- */
int main(int argc, char* argv[])
{
	default_filename();

	if(argc==1) {
		if(!file_exist(recfile.name)) {
			error("file not found\n");
		}
		show_diff(recfile.name);
		return 0;
	}
	int opt = getopt(argc, argv, "wn:");

	switch(opt) {
	case 'w':
		time_stamp();
		time_stamp_log();
		break;
	case 'n':
		{
			if(file_exist(optarg)) error("file already exist\n");
			other_filename(optarg);
			time_stamp();
		}
		break;
	case '?':
		error (
				"Unknow option\n"
				"Usage\n-w:\t write a current date and time\n"
				"-n pathname:\t set file name\n" 
			  );
	}
	return 0;
}
/* ------------------------------------------------------------------------- */

/*
------------------------------------------------------------------------- */
void init_file_location(void)
{
	memset(&recfile, 0, sizeof (struct file_location));
	memset(&recfile_log, 0, sizeof (struct file_location));
}
/* デフォルトのファイル名を設定
	プログラム内部で一貫して使われるファイル名となる
------------------------------------------------------------------------- */
void default_filename(void)
{
	strncpy(recfile.name, "record", FILENAME_LEN);
	strncpy(recfile_log.name, "record.log", FILENAME_LEN);
}
/*
------------------------------------------------------------------------- */
void other_filename(const char* path)
{
	init_file_location();
	const int len = strlen(path);
	const char suffix[] = ".log";

	if(len < FILENAME_LEN) {
		strncpy(recfile.name, path, len); 
	}
	if(len < FILENAME_LEN - sizeof suffix) {
		strncpy(recfile_log.name, path, len); 
		strncat(recfile_log.name, suffix, sizeof suffix);
	}
}
/* 関数呼び出し時の今日の日付・時刻
	・フォーマット 例：2001/04/01 13:05:00
	必要：引数へはサイズDATE_LEN分が割り当てられた配列を渡す
	注意：引数へポインタ、文字列定数を渡すとエラー
-------------------------------------------------------------------------- */
void date_time(char* dt)
{
/* TODO: バッファ（緩衝材）の用意
	引数にポインタを渡された時に対策 */
	time_t date = time(NULL);
	strftime(dt, DATE_LEN, "%Y/%m/%d %H:%M:%S", localtime(&date));
}
/* ファイルの存在チェック 
	・存在する場合1を返し、存在しない場合は0（ゼロ）を返す
------------------------------------------------------------------------- */
int file_exist(const char* path)
{
	struct stat finfo;
	if(stat(path, &finfo) == -1) { 
		return 0;
	}
	return 1;
}
/* ファイルへ日付、時刻を書き込む（新規）
	・更新時に古いファイル内容は破棄する
	・ファイルには常に1行のみ
	・データフォーマット："yyyy/mm/dd[space]hh:mm:ss"
------------------------------------------------------------------------- */
void time_stamp(void)
{
	char date[DATE_LEN] = {0};
	date_time(date);

	FILE* writer = NULL; 
	
	if((writer = fopen(recfile.name, "w")) == NULL) {
		error("file cannot open\n");
	}
 	fprintf(writer,"%s", date);
	fclose(writer);
}
/* ファイルへ日付、時刻を書き込む（追加）
	・タイムスタンプを複数積んでおく
	・複数行からなる
	・データは末尾から追加
	・データフォーマット："yyyy/mm/dd[space]hh:mm:ss\n"
------------------------------------------------------------------------- */
void time_stamp_log(void)
{
/* TODO: 書き込むファイルの行数に制限*/
	char date[DATE_LEN] = {0};
	date_time(date);

	FILE* writelog = NULL; 

	if((writelog = fopen(recfile_log.name, "a")) == NULL) {
		error("file cannot open\n");
	}
 	fprintf(writelog, "%s\n", date);
	fclose(writelog);
}
/* ファイルから日付・時刻を読み込む
	・読み取りは行単位で行う
	・引数lineへは割り当て済みの配列を渡す
	・読み取った行数を返す
------------------------------------------------------------------------- */
int read_line(char* line, FILE* src)
{
	if(src == NULL) return -1;

	int i, c;
	for(i = 0; i<MAXLEN-1 && (c=fgetc(src))!=EOF && c!='\n'; ++i) {
		line[i] = c;
		if(c=='\n') {
			line[i++] = c;
		}
	}
	line[i] = '\0';
	fclose(src);
	
	return i;
}
/* 時間の差を表示
	・現在の日付・時刻とファイルに記録された時間の差（単位：秒）
	注：実際の時間差の計算と出力はprint_level関数で行う。
------------------------------------------------------------------------- */
void show_diff(const char* name)
{
	char str[MAXLEN] = {0};
	FILE* reader = NULL;

	if((reader = fopen(name, "r")) == NULL)  {
		error("file cannot open\n");
	}
	if((read_line(str,reader)) <= 0) {
		error("can not read the data from file");
	}
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
/* TODO: フォーマット入力の見直し
	１文字単位の読み取りを検討 */
	char buf[DATE_LEN] = {0};
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
	・経過時間（秒）はshow_diff関数から渡される
	・月単位はなし
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
