#include "Trace.h"
#include <tchar.h>

void _trace(LPCTSTR lpszFormat, ...)
{
	TCHAR lpszBuffer[1000];

	va_list fmtList;
	va_start(fmtList, lpszFormat);
	_vstprintf_s(lpszBuffer, lpszFormat, fmtList);
	va_end(fmtList);
	::OutputDebugString(lpszBuffer);
}


void TraceTimingPoint()
{
	INT64 t;
	QueryPerformanceCounter((LARGE_INTEGER*)&t);
	TRACE(_T("%lfms\n"), (float)((float)(t % 10000000) / 100) * 0.01f);
}
