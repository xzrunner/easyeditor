#include "StackTrace.h"

#include <stackwalker.h>
#include <fstream>
#include <tchar.h>

#include <Windows.h>

namespace ee
{

class StackWalkerToConsole : public StackWalker
{
protected:
	virtual void OnOutput(LPCSTR szText) 
	{
		std::ofstream fout("stacktrace.txt", std::ios::app);

		fout << szText << std::endl;

		fout.close();
	}
};

static LONG WINAPI ExpFilter(EXCEPTION_POINTERS* pExp, DWORD dwExpCode)
{
	//StackWalker sw;  // output to default (Debug-Window)
	StackWalkerToConsole sw;  // output to the console
	sw.ShowCallstack(GetCurrentThread(), pExp->ContextRecord);
	return EXCEPTION_EXECUTE_HANDLER;
}

//////////////////////////////////////////////////////////////////////////

#if defined _M_X64 || defined _M_IX86
static BOOL PreventSetUnhandledExceptionFilter()
{
	HMODULE hKernel32 = LoadLibrary(_T("kernel32.dll"));
	if (hKernel32 == NULL) return FALSE;
	void *pOrgEntry = GetProcAddress(hKernel32, "SetUnhandledExceptionFilter");
	if (pOrgEntry == NULL) return FALSE;

#ifdef _M_IX86
	// Code for x86:
	// 33 C0                xor         eax,eax  
	// C2 04 00             ret         4 
	unsigned char szExecute[] = { 0x33, 0xC0, 0xC2, 0x04, 0x00 };
#elif _M_X64
	// 33 C0                xor         eax,eax 
	// C3                   ret  
	unsigned char szExecute[] = { 0x33, 0xC0, 0xC3 };
#else
#error "The following code only works for x86 and x64!"
#endif

	DWORD dwOldProtect = 0;
	BOOL bProt = VirtualProtect(pOrgEntry, sizeof(szExecute), 
		PAGE_EXECUTE_READWRITE, &dwOldProtect);

	SIZE_T bytesWritten = 0;
	BOOL bRet = WriteProcessMemory(GetCurrentProcess(),
		pOrgEntry, szExecute, sizeof(szExecute), &bytesWritten);

	if ( (bProt != FALSE) && (dwOldProtect != PAGE_EXECUTE_READWRITE))
	{
		DWORD dwBuf;
		VirtualProtect(pOrgEntry, sizeof(szExecute), dwOldProtect, &dwBuf);
	}
	return bRet;
}
#else
#pragma message("This code works only for x86 and x64!")
#endif

static LONG __stdcall CrashHandlerExceptionFilter(EXCEPTION_POINTERS* pExPtrs)
{
#ifdef _M_IX86
	if (pExPtrs->ExceptionRecord->ExceptionCode == EXCEPTION_STACK_OVERFLOW)
	{
		static char MyStack[1024*128];  // be sure that we have enought space...
		// it assumes that DS and SS are the same!!! (this is the case for Win32)
		// change the stack only if the selectors are the same (this is the case for Win32)
		//__asm push offset MyStack[1024*128];
		//__asm pop esp;
		__asm mov eax,offset MyStack[1024*128];
		__asm mov esp,eax;
	}
#endif

	StackWalkerToConsole sw;  // output to console
	sw.ShowCallstack(GetCurrentThread(), pExPtrs->ContextRecord);
	TCHAR lString[500];
	_stprintf_s(lString,
		_T("*** Unhandled Exception! See console output for more infos!\n")
		_T("   ExpCode: 0x%8.8X\n")
		_T("   ExpFlags: %d\n")
		_T("   ExpAddress: 0x%8.8X\n")
		_T("   Please report!"),
		pExPtrs->ExceptionRecord->ExceptionCode,
		pExPtrs->ExceptionRecord->ExceptionFlags,
		pExPtrs->ExceptionRecord->ExceptionAddress);
	FatalAppExit(-1, lString);
	return EXCEPTION_CONTINUE_SEARCH;
}

static TCHAR s_szExceptionLogFileName[_MAX_PATH] = _T("\\exceptions.log");  // default
static BOOL s_bUnhandledExeptionFilterSet = FALSE;

void StackTrace::InitUnhandledExceptionFilter()
{
	TCHAR szModName[_MAX_PATH];
	if (GetModuleFileName(NULL, szModName, sizeof(szModName)/sizeof(TCHAR)) != 0)
	{
		_tcscpy_s(s_szExceptionLogFileName, szModName);
		_tcscat_s(s_szExceptionLogFileName, _T(".exp.log"));
	}
	if (s_bUnhandledExeptionFilterSet == FALSE)
	{
		// set global exception handler (for handling all unhandled exceptions)
		SetUnhandledExceptionFilter(CrashHandlerExceptionFilter);
#if defined _M_X64 || defined _M_IX86
		PreventSetUnhandledExceptionFilter();
#endif
		s_bUnhandledExeptionFilterSet = TRUE;
	}
}

}