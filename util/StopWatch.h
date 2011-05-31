//
//
// This is a very simple timer class based on timeGetTime() c function.
// http://tdistler.com/2010/06/27/high-performance-timing-on-linux-windows
//
//
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
		#ifdef _WINDOWS
		unsigned long       m_startTime; // time in ms
		#endif
		#ifdef linux
		float               m_startTime; // time in s
		#endif
		float               m_frequency;
	};
}
