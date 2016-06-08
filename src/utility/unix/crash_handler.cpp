/*
 * crash_handler.cpp
 *
 *  Created on: 28 nov. 2015
 *      Author: Julien
 */

#include "crash_handler.h"
/*
void crash_handler(int sig)
{
	if(sig != SIGINT)
	{
		LOG(FATAL) << "Application crashed!";
		LOG(FATAL) << "Received signal: " << sig;

		switch (sig)
		{
			case SIGABRT: LOG(FATAL) << "SIGABRT" << ": " << "Program was abnormally terminated."; break;
			case SIGFPE : LOG(FATAL) << "SIGFPE"  << ": " << "Arithmetic operation issue such as division by zero or operation resulting in overflow."; break;
			case SIGILL : LOG(FATAL) << "SIGILL"  << ": " << "Generally due to a corruption in the code or to an attempt to execute data."; break;
			case SIGSEGV: LOG(FATAL) << "SIGSEGV" << ": " << "Program is trying to read an invalid (unallocated, deleted or corrupted) or inaccessible memory."; break;
			//case SIGINT : LOG(FATAL) << "SIGINT"  << ": " << "Interruption generated (generally) by user or operating system."; break;
			case SIGTRAP: LOG(FATAL) << "SIGTRAP" << ": " << "Trace/breakpoint trap."; break;
		}

		void *array[10];
		size_t size;
		char **strings;
		size = backtrace(array, 10);
		strings = backtrace_symbols (array, size);
		LOG(FATAL) << "Stacktrace:";
		for (size_t i = 0; i < size; i++)
			LOG(FATAL) << strings[i];
	}
	else
	{
		LOG(FATAL) << "Exit application!";
	}

	log_file.close();
	cerr << flush;
}
*/
