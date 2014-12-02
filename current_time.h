#ifndef CURRENT_TIME_H
#define CURRENT_TIME_H

#include <stdio.h>
#include <time.h>
#include "util.h"

#define DATE_LEN 11	/* yyyy/mm/dd  */
#define TIME_LEN 9	/* hh:mm:ss */

void today(char* dt);
void now(char* tv);

#endif
