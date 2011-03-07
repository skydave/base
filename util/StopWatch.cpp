/*---------------------------------------------------------------------

This is a very simple timer class based on timeGetTime() c function.

----------------------------------------------------------------------*/
#include "StopWatch.h"

// add the multimedia library of windows
#pragma message("     _Adding library: winmm.lib" ) 
#pragma comment( lib, "winmm.lib" )

namespace dk
{
	namespace util
	{
		//
		//
		//
		StopWatch::StopWatch()
		{
			timeBeginPeriod(1);
		}


		//
		//
		//
		StopWatch::~StopWatch()
		{
			timeEndPeriod(1);
		}

		//
		//
		//
		void StopWatch::start()
		{
			startTime = timeGetTime();
		}


		//
		//
		//
		float StopWatch::elapsedSeconds()
		{
			return  (timeGetTime() - startTime)/1000.0f;
		}
	}
}