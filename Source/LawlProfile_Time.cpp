#include "LawlProfile_Time.h"
	
#ifdef _WIN32
#include <windows.h>
#endif

BEGIN_LAWLPROFILE

namespace LP_Time
{
	TimeVal GetSysTime()
	{
		TimeVal t;

	#ifdef _WIN32
		LARGE_INTEGER temp;
		QueryPerformanceCounter(&temp);
		t = temp.QuadPart;
	#endif

		return t;
	}

	double ConvertSysTime(TimeVal t)
	{
		double time;

	#ifdef _WIN32
		LARGE_INTEGER temp;
		QueryPerformanceFrequency(&temp);
		time = t / (double)temp.QuadPart;
	#endif

		return time;
	}

	double GetTimeMS()
	{
		return ConvertSysTime(GetSysTime()) * 1000.0;
	}
}

END_LAWLPROFILE