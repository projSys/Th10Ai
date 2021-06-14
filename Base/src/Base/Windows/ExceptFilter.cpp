#include "Base/Windows/ExceptFilter.h"

#include <stdlib.h>
#include <new.h>
#include <eh.h>
#include <signal.h>

#include "Base/Windows/ExceptTrace.h"

namespace base
{
	namespace win
	{
		void ExceptFilter::SetProcessExceptionHandlers()
		{
			// Install top-level SEH handler
			SetUnhandledExceptionFilter(UnhandledExceptionFilter);

			// 在__except/catch之前，捉得太多
			//AddVectoredExceptionHandler(1, VectoredExceptionHandler);

			// 在UnhandledExceptionFilter之后，捉得太少
			//AddVectoredContinueHandler(1, VectoredContinueHandler);

			// Catch new operator memory allocation exceptions
			_set_new_handler(NewHandler);

			// 设置处理malloc内存失败异常跟new行为一样
			_set_new_mode(1);

			// Catch pure virtual function calls.
			// Because there is one _purecall_handler for the whole process,
			// calling this function immediately impacts all threads. The last
			// caller on any thread sets the handler.
			// http://msdn.microsoft.com/en-us/library/t296ys27.aspx
			_set_purecall_handler(PureCallHandler);

			// Catch invalid parameter exceptions.
			_set_invalid_parameter_handler(InvalidParameterHandler);

			// Setup C++ signal handlers
			signal(SIGABRT, SigabrtHandler);
			//signal(SIGINT, SigintHandler);
			//signal(SIGBREAK, SigbreakHandler);
			//signal(SIGTERM, SigtermHandler);
		}

		void ExceptFilter::SetThreadExceptionHandlers()
		{
			// Catch terminate() calls.
			// In a multithreaded environment, terminate functions are maintained
			// separately for each thread. Each new thread needs to install its own
			// terminate function. Thus, each thread is in charge of its own termination handling.
			// http://msdn.microsoft.com/en-us/library/t6fk7h29.aspx
			set_terminate(TerminateHandler);

			// Catch unexpected() calls.
			// In a multithreaded environment, unexpected functions are maintained
			// separately for each thread. Each new thread needs to install its own
			// unexpected function. Thus, each thread is in charge of its own unexpected handling.
			// http://msdn.microsoft.com/en-us/library/h46t5b69.aspx
			set_unexpected(UnexpectedHandler);

			// Setup C++ signal handlers
			//signal(SIGILL, SigillHandler);
			//signal(SIGFPE, SigfpeHandler);
			//signal(SIGSEGV, SigsegvHandler);
		}

		LONG ExceptFilter::UnhandledExceptionFilter(EXCEPTION_POINTERS* ep)
		{
			Handle(ep);
			return EXCEPTION_CONTINUE_SEARCH;
		}

		LONG ExceptFilter::VectoredExceptionHandler(EXCEPTION_POINTERS* ep)
		{
			Handle(ep);
			return EXCEPTION_CONTINUE_SEARCH;
		}

		LONG ExceptFilter::VectoredContinueHandler(EXCEPTION_POINTERS* ep)
		{
			Handle(ep);
			return EXCEPTION_CONTINUE_SEARCH;
		}

		void ExceptFilter::TerminateHandler()
		{
			Raise();
		}

		void ExceptFilter::UnexpectedHandler()
		{
			Raise();
		}

		int ExceptFilter::NewHandler(size_t size)
		{
			Raise();
			return 0;
		}

		void ExceptFilter::PureCallHandler()
		{
			Raise();
		}

		void ExceptFilter::InvalidParameterHandler(
			const wchar_t* expr, const wchar_t* func,
			const wchar_t* file, unsigned int line,
			uintptr_t reserved)
		{
			Raise();
		}

		void ExceptFilter::SigabrtHandler(int sig)
		{
			Raise();
		}

		void ExceptFilter::SigintHandler(int sig)
		{
			Raise();
		}

		void ExceptFilter::SigbreakHandler(int sig)
		{
			Raise();
		}

		void ExceptFilter::SigtermHandler(int sig)
		{
			Raise();
		}

		void ExceptFilter::SigillHandler(int sig)
		{
			Raise();
		}

		void ExceptFilter::SigfpeHandler(int sig)
		{
			Raise();
		}

		void ExceptFilter::SigsegvHandler(int sig)
		{
			Raise();
		}

		void ExceptFilter::Raise()
		{
			__try
			{
				RaiseException(0, 0, 0, nullptr);
			}
			__except (Filter(GetExceptionInformation()))
			{
			}
		}

		LONG ExceptFilter::Filter(EXCEPTION_POINTERS* ep)
		{
			Handle(ep);
			// 从异常处重新执行
			//return EXCEPTION_CONTINUE_EXECUTION;
			// 从__except块后执行
			return EXCEPTION_EXECUTE_HANDLER;
		}

		void ExceptFilter::Handle(EXCEPTION_POINTERS* ep)
		{
			g_exceptTrace.handle(ep);
			ExitProcess(1);
		}
	}
}
