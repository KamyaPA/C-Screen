#include <stdio.h>
#include <stdarg.h>

#define LOGFILE_NAME "./game.log"

FILE* GAME_LOG;

int log_init (void){
	GAME_LOG = fopen(LOGFILE_NAME, "w");
	if(GAME_LOG == NULL) 
		{ printf("Couldn't initialize log"); return 0;}	
	else return 1;
}

int log_term (void){
	return fclose(GAME_LOG);
}

/* Prints to the log file */
int printlog (FILE* logfile, char * message) {
	int rtn = fputs(message, logfile);
	return rtn;
}

