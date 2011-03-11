/*---------------------------------------------------------------------

This is a very simple timer class based on timeGetTime() c function.

----------------------------------------------------------------------*/
#pragma once
#ifdef _WINDOWS
#include <windows.h>
#include <mmsystem.h>
#endif


namespace base
{
	//
	// This class is able to measure elapsed time from a certain start point
	//
	class StopWatch
	{
	public:
		StopWatch();
		~StopWatch();
		void start();
		float elapsedSeconds();

	private:
		unsigned long         startTime;
	};
}
