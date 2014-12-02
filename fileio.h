#ifndef FILEIO_H
#define FILEIO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "util.h"

#define SECONDS 60
#define ONE_MIN 60
#define ONE_HOUR 60
#define ONE_DAY 24
#define ONE_YEAR 365

#define MAXLEN 100	/* for read_line() */
#define FILENAME_LEN 256

/* ------------------------------------------------------------------------- */
struct file_location{ char name[FILENAME_LEN]; } recfile, recfile_log;
/* ------------------------------------------------------------------------- */


#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int file_exist(const char* path);
void time_stamp(void);
char* read_line(char* line);
void show_diff(void);
time_t passed_date(struct tm old, const char* date);
void print_level(long diffsec);

#endif
