#include "Base/Windows/ExceptTrace.h"

#include <DbgHelp.h>
#include <sstream>
#include <fstream>

#include "Base/Log.h"
#include "Base/Logger.h"

#pragma comment(lib, "DbgHelp.lib")

namespace base
{
	namespace win
	{
		ExceptTrace g_exceptTrace;

		ExceptTrace::ExceptTrace() :
			m_context{},
			m_frames{},
			m_size(0)
		{
		}

		void ExceptTrace::trace(EXCEPTION_POINTERS* info)
		{
			std::unique_lock<std::mutex> lock(m_mutex);

			g_logger.flush();

			memcpy_s(&m_context, sizeof(CONTEXT), info->ContextRecord, sizeof(CONTEXT));

			f1();
			f2();
		}

		void ExceptTrace::f1()
		{
			HANDLE process = GetCurrentProcess();
			HANDLE thread = GetCurrentThread();

#if defined(_M_IX86)
			DWORD machineType = IMAGE_FILE_MACHINE_I386;
#elif defined(_M_X64) 
			DWORD machineType = IMAGE_FILE_MACHINE_AMD64;
#else
#error "This platform is not supported."
#endif

#if defined(_M_IX86)
			STACKFRAME64 stackFrame = {};
			stackFrame.AddrPC.Offset = m_context.Eip;
			stackFrame.AddrPC.Mode = AddrModeFlat;
			stackFrame.AddrFrame.Offset = m_context.Ebp;
			stackFrame.AddrFrame.Mode = AddrModeFlat;
			stackFrame.AddrStack.Offset = m_context.Esp;
			stackFrame.AddrStack.Mode = AddrModeFlat;
#elif defined(_M_X64) 
			STACKFRAME64 stackFrame = {};
			stackFrame.AddrPC.Offset = m_context.Rip;
			stackFrame.AddrPC.Mode = AddrModeFlat;
			stackFrame.AddrFrame.Offset = m_context.Rbp;
			stackFrame.AddrFrame.Mode = AddrModeFlat;
			stackFrame.AddrStack.Offset = m_context.Rsp;
			stackFrame.AddrStack.Mode = AddrModeFlat;
#else
#error "This platform is not supported."
#endif

			for (DWORD64 i = 0; i < BUFFER_SIZE; ++i)
			{
				if (StackWalk64(machineType, process, thread, &stackFrame, &m_context,
					nullptr, SymFunctionTableAccess64, SymGetModuleBase64, nullptr))
				{
					m_frames[i] = stackFrame.AddrPC.Offset;
					++m_size;
				}
				else
				{
					break;
				}
			}
		}

		void ExceptTrace::f2()
		{
			HANDLE process = GetCurrentProcess();
			HANDLE thread = GetCurrentThread();

			SymSetOptions(SYMOPT_LOAD_LINES);
			SymInitialize(process, nullptr, TRUE);

			BYTE buffer[sizeof(SYMBOL_INFO) + sizeof(CHAR) * (MAX_SYM_NAME + 1)] = {};
			PSYMBOL_INFO symbol = reinterpret_cast<PSYMBOL_INFO>(buffer);
			symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
			symbol->MaxNameLen = MAX_SYM_NAME;

			IMAGEHLP_LINE64 line = {};
			line.SizeOfStruct = sizeof(IMAGEHLP_LINE64);

			DWORD64 symDisplacement = 0;
			DWORD lineDisplacement = 0;

			std::ostringstream oss;
			oss << "StackTrace:\n";
			for (DWORD64 i = 0; i < m_size; ++i)
			{
				DWORD64 address = m_frames[i];

				SymFromAddr(process, address, &symDisplacement, symbol);
				SymGetLineFromAddr64(process, address, &lineDisplacement, &line);

				oss << i + 1
					<< " in " << symbol->Name
					<< " at " << line.FileName
					<< " : " << line.LineNumber << '\n';
			}

			SymCleanup(process);

			BASE_LOG(fatal) << oss.str() << std::endl;
			//BASE_LOG_FLUSH;
		}
	}
}
