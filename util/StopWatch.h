/*---------------------------------------------------------------------

This is a very simple timer class based on timeGetTime() c function.

----------------------------------------------------------------------*/
#pragma once
#include <windows.h>
#include <mmsystem.h>



namespace dk
{
	namespace util
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
			DWORD                 startTime;
		};
	}
}
