#include "Log.h"

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>


/* mode = i / e : info / error */
static void vfPrint_Info(char mode, const char* format, va_list vp);


void Log_Info(const char* format, ...)
{
	va_list arg;
	va_start(arg, format);
	
	vfPrint_Info('i', format, arg);

	va_end(arg);
}


void Log_Error(const char* format, ...)
{
	va_list arg;
	va_start(arg, format);
	
	vfPrint_Info('e', format, arg);

	va_end(arg);
	exit(1);
}

/* static functions: */
static void vfPrint_Info(char mode, const char* format, va_list vp)
{
	if(mode == 'i' || mode == 'e') {
		FILE* stream = (mode == 'i')? stdout : stderr;

		fprintf(stream, (mode == 'i')? "INFO: " : "ERROR: ");
		vfprintf(stream, format, vp);
		fprintf(stream, "\n");
	}

}
