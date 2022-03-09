#ifndef LOG_HG
#define LOG_HG

#include <stdio.h>

/* Print out the curret file, and line number to stderr */
#define Log_ErrorLine()													\
	fprintf(stderr, "ERROR in file %s line %d\n", __FILE__, __LINE__);	\


/* Print out the curret file, and line number to stdout */
#define Log_InfoLine()													\
	fprintf(stdout, "INFO in file %s line %d\n", __FILE__, __LINE__);	\


/* Print out formated output to stdout.
 * For more specification, read manual page of printf.
 * */
void Log_Info(const char* format, ...);


/* Print out formated output to stderr.
 * After that, it exits the program.
 * For more specification, read manual page of printf.
 * */
void Log_Error(const char* format, ...);

#endif
