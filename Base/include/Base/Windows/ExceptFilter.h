#pragma once

#include "Base/Windows/Common.h"

namespace base
{
	namespace win
	{
		// https://www.codeproject.com/Articles/207464/Exception-Handling-in-Visual-Cplusplus
		class ExceptFilter
		{
		public:
			static void SetProcessExceptionHandlers();
			static void SetThreadExceptionHandlers();

		private:
			static LONG WINAPI UnhandledExceptionFilter(EXCEPTION_POINTERS* info);
			//static LONG NTAPI VectoredExceptionHandler(EXCEPTION_POINTERS* info);
			//static LONG NTAPI VectoredContinueHandler(EXCEPTION_POINTERS* info);

			static void __CRTDECL TerminateHandler();
			static void __CRTDECL UnexpectedHandler();

			static int __CRTDECL NewHandler(size_t size);
			static void __cdecl PurecallHandler();
			static void __cdecl InvalidParameterHandler(
				const wchar_t* expr, const wchar_t* func,
				const wchar_t* file, unsigned int line,
				uintptr_t reserved);

			static void __CRTDECL SigabrtHandler(int sig);
			static void __CRTDECL SigintHandler(int sig);
			static void __CRTDECL SigbreakHandler(int sig);
			static void __CRTDECL SigtermHandler(int sig);

			static void __CRTDECL SigillHandler(int sig);
			static void __CRTDECL SigfpeHandler(int sig);
			static void __CRTDECL SigsegvHandler(int sig);

			static void Raise();
			static LONG Filter(EXCEPTION_POINTERS* info);

			static void Trace(EXCEPTION_POINTERS* info);
		};
	}
}
