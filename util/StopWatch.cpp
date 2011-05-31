/*---------------------------------------------------------------------

This is a very simple timer class based on timeGetTime() c function.

----------------------------------------------------------------------*/
#include "StopWatch.h"
#include <iostream>
#ifdef _WINDOWS
// add the multimedia library of windows
#pragma message("     _Adding library: winmm.lib" ) 
#pragma comment( lib, "winmm.lib" )
#endif

#ifdef linux
#include <time.h>
#endif
namespace base
{
	//
	//
	//
	StopWatch::StopWatch()
	{
#ifdef _WINDOWS
		timeBeginPeriod(1);
#endif
#ifdef linux

		struct timespec spec;
		/*
		clockid_t types[] = { CLOCK_REALTIME, CLOCK_MONOTONIC, CLOCK_PROCESS_CPUTIME_ID, CLOCK_THREAD_CPUTIME_ID, (clockid_t) - 1 };

		int i = 0;
		for ( i; types[i] != (clockid_t) - 1; i++ )
		{
			if ( clock_getres( types[i], &spec ) != 0 )
				std::cout << "Timer %d not supported. "<< types[i] << std::endl;
			else
				std::cout << "Timer: " << i << ", Seconds: " << spec.tv_sec << " Nanos: " << spec.tv_nsec << std::endl;
		}
		*/
		clock_getres( CLOCK_MONOTONIC, &spec );
		m_frequency = spec.tv_nsec;
#endif
	}


	//
	//
	//
	StopWatch::~StopWatch()
	{
#ifdef _WINDOWS
		timeEndPeriod(1);
#endif
#ifdef linux
#pragma error ("missing implementation for linux")
#endif
	}

	//
	//
	//
	void StopWatch::start()
	{
#ifdef _WINDOWS
		m_startTime = timeGetTime();
#endif
#ifdef linux
		struct timespec spec;
		clock_gettime(CLOCK_MONOTONIC, &spec);
		m_startTime = (float)(spec.tv_sec + (spec.tv_nsec/1000000000.0f));
#endif
	}


	//
	//
	//
	float StopWatch::elapsedSeconds()
	{
#ifdef _WINDOWS
		return  (timeGetTime() - m_startTime)/1000.0f; // convert from ms in s
#endif
#ifdef linux
		struct timespec spec;
		clock_gettime(CLOCK_MONOTONIC, &spec);
		return (spec.tv_sec + (spec.tv_nsec/1000000000.0f)) - m_startTime;
#endif
	}
}
