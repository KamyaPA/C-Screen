#include <stdio.h>
#include <stdarg.h>

/* A declaration of the log files*/
extern FILE* GAME_LOG;

/* Creates a log file on the given path */
int log_init (void);
int log_term (void);

/* prints to a specific log file */
int printlog (FILE* logfile, char * message);


