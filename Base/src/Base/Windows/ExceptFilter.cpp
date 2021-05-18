#include "Base/Windows/ExceptFilter.h"

#include <stdlib.h>
#include <new.h>
#include <eh.h>
#include <signal.h>

namespace base
{
	namespace win
	{
		void ExceptFilter::SetProcessExceptionHandlers()
		{
			// Install top-level SEH handler
			SetUnhandledExceptionFilter(StructuredExceptionHandler);

			// before the system begins unwinding the stack.
			//AddVectoredExceptionHandler(1, VectoredExceptionHandler);

			// Catch new operator memory allocation exceptions
			_set_new_handler(NewHandler);

			// 设置处理malloc内存失败异常跟new行为一样
			_set_new_mode(1);

			// Catch pure virtual function calls.
			// Because there is one _purecall_handler for the whole process,
			// calling this function immediately impacts all threads. The last
			// caller on any thread sets the handler.
			// http://msdn.microsoft.com/en-us/library/t296ys27.aspx
			_set_purecall_handler(PurecallHandler);

			// Catch invalid parameter exceptions.
			_set_invalid_parameter_handler(InvalidParameterHandler);
			_set_thread_local_invalid_parameter_handler(InvalidParameterHandler);

			// Setup C++ signal handlers
			signal(SIGABRT, SignalHandler);
			signal(SIGINT, SignalHandler);
			signal(SIGBREAK, SignalHandler);
			signal(SIGTERM, SignalHandler);
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
			signal(SIGFPE, SignalHandler);
			signal(SIGILL, SignalHandler);
			signal(SIGSEGV, SignalHandler);
		}

		LONG ExceptFilter::StructuredExceptionHandler(EXCEPTION_POINTERS* info)
		{
			Filter(info);
			return EXCEPTION_EXECUTE_HANDLER;
		}

		//LONG ExceptFilter::VectoredExceptionHandler(EXCEPTION_POINTERS* info)
		//{
		//	Filter(info);
		//	return EXCEPTION_CONTINUE_SEARCH;
		//}

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

		void ExceptFilter::PurecallHandler()
		{
			Raise();
		}

		void ExceptFilter::InvalidParameterHandler(const wchar_t* expr,
			const wchar_t* func, const wchar_t* file, unsigned int line,
			uintptr_t reserved)
		{
			Raise();
		}

		void ExceptFilter::SignalHandler(int sig)
		{
			Raise();
		}

		void ExceptFilter::Raise()
		{
			__try
			{
				RaiseException(0, 0, 0, nullptr);
			}
			//__except (EXCEPTION_EXECUTE_HANDLER)
			__except (Filter(GetExceptionInformation()))
			{
			}
		}

		LONG ExceptFilter::Filter(EXCEPTION_POINTERS* info)
		{
			// 在异常指令处继续执行
			//return EXCEPTION_CONTINUE_EXECUTION;
			// 执行__except块后的代码
			return EXCEPTION_EXECUTE_HANDLER;
		}
	}
}
