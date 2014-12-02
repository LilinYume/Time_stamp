#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "current_time.c"
#include "fileio.c"
#include "util.c"

/* ------------------------------------------------------------------------- */
int main(int argc, char* argv[])
{
	/* デフォルトのファイル名を設定
		プログラム内部で一貫して使われるファイル名となる */
	strncpy(recfile.name, "record", FILENAME_LEN);
	strncpy(recfile_log.name, "record.log", FILENAME_LEN);

	if(argc==1) {
		if(!file_exist(recfile.name)) {
			error("file not found\n");
			return 1;
		}
		show_diff();
	}
	int opt = getopt(argc, argv, "wn:");

	switch(opt) {
	case 'w':
		time_stamp();
		break;
	case 'n':
		{
			const char* path = optarg;
			if(file_exist(path)) error("file already exist\n");

			const int len = strlen(path);
			if(len < FILENAME_LEN) {
				memset(&recfile, 0, sizeof (struct file_location));
				strncpy(recfile.name, path, len); 
			}
		time_stamp();
		}
		break;
	case '?':
		error (
				"Unknow option\n"
				"Usage\n-w:\t write a current date and time\n"
				"-n pathname:\t set file name\n" 
			  );
		return 0;
	}
	return 0;
}
/* ------------------------------------------------------------------------- */
