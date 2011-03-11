/*---------------------------------------------------------------------

This is a very simple timer class based on timeGetTime() c function.

----------------------------------------------------------------------*/
#include "StopWatch.h"

#ifdef _WINDOWS
// add the multimedia library of windows
#pragma message("     _Adding library: winmm.lib" ) 
#pragma comment( lib, "winmm.lib" )
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
#pragma error ("missing implementation for linux")
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
		startTime = timeGetTime();
#endif
#ifdef linux
#pragma error ("missing implementation for linux")
#endif
	}


	//
	//
	//
	float StopWatch::elapsedSeconds()
	{
#ifdef _WINDOWS
		return  (timeGetTime() - startTime)/1000.0f;
#endif
	}
}
