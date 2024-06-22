#pragma once
#include <windows.h>

#if defined(DEBUG) | defined(_DEBUG)
void _trace(LPCTSTR lpszFormat, ...);
#define TRACE _trace
#else
#define TRACE false && _trace
#endif


void TraceTimingPoint();