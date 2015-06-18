#ifndef _DRAG2D_STACK_TRACE_H_
#define _DRAG2D_STACK_TRACE_H_

#include <Windows.h>

namespace d2d
{

class StackTrace
{
public:
	static LONG WINAPI ExpFilter(EXCEPTION_POINTERS* pExp, DWORD dwExpCode);

	static void InitUnhandledExceptionFilter();

}; // StackTrace

}

#endif // _DRAG2D_STACK_TRACE_H_