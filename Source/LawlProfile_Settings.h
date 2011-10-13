#ifndef LAWLPROFILE_SETTINGS_H
#define LAWLPROFILE_SETTINGS_H

// Set to 1 to enable the profiler and to 0 to disable it
//
#define LAWLPROFILE_PROF_ENABLED 0

// Define the path to the profiler's results folder
//
#define LAWLPROFILE_RESULTS_FOLDER "\\Profiler"

// Set to 1 if you want LawlProfile to be inside the LawlProfile namespace
// and 0 if not.
//
#if 1
	#define BEGIN_LAWLPROFILE namespace LawlProfile {
	#define END_LAWLPROFILE }
#else
	#define BEGIN_LAWLPROFILE
	#define END_LAWLPROFILE
#endif

#endif