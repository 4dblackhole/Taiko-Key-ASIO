#pragma once
#include <windows.h>

#if defined(DEBUG) | defined(_DEBUG)
void _trace(LPCTSTR lpszFormat, ...);
#define TRACE _trace
#else
#define TRACE(expression) ((void)0)
#endif

#if defined(DEBUG) | defined(_DEBUG)
void TraceTimingPoint();
#endif