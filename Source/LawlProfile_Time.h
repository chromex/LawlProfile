#ifndef LAWLPROFILE_TIME_H
#define LAWLPROFILE_TIME_H

#include "LawlProfile_Settings.h"

BEGIN_LAWLPROFILE

namespace LP_Time
{
	typedef unsigned long long TimeVal;
	TimeVal GetSysTime();
	double ConvertSysTime(TimeVal t);
	double GetTimeMS();
}

END_LAWLPROFILE

#endif