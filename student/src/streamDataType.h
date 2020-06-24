#pragma once


// our socketstaream data structure 
// the first four bytes is the raw data size
// now only pixels data compressed
// the five byte represents the type of data , such as pixels, audio, likewise
// we appoint 
#define CONNECTIONREQUESTTYPE 1
#define CONNECTIONRESPONSETYPE 2
#define PIXELTYPE 3
#define AUDIOTYPE 4
