#include "Base/Windows/ExceptTrace.h"

#include <DbgHelp.h>
#include <sstream>
#include <iomanip>

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

		void ExceptTrace::handle(EXCEPTION_POINTERS* ep)
		{
			std::unique_lock<std::mutex> lock(m_mutex);
			memcpy(&m_context, ep->ContextRecord, sizeof(CONTEXT));
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

			m_size = 0;
			for (DWORD64 i = 0; i < BUFFER_SIZE; ++i)
			{
				if (!StackWalk64(machineType, process, thread, &stackFrame, &m_context,
					nullptr, SymFunctionTableAccess64, SymGetModuleBase64, nullptr))
					break;

				if (stackFrame.AddrReturn.Offset == 0)
					break;

				m_frames[i] = stackFrame.AddrPC.Offset;
				++m_size;
			}
		}

		void ExceptTrace::f2()
		{
			//todo 线程安全
			HANDLE process = GetCurrentProcess();

			SymSetOptions(SYMOPT_LOAD_LINES);
			SymInitialize(process, nullptr, TRUE);

			DWORD64 symDisplacement = 0;
			BYTE buffer[sizeof(SYMBOL_INFO) + sizeof(CHAR) * (MAX_SYM_NAME + 1)] = {};
			SYMBOL_INFO* symbol = reinterpret_cast<SYMBOL_INFO*>(buffer);
			symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
			symbol->MaxNameLen = MAX_SYM_NAME;

			DWORD lineDisplacement = 0;
			IMAGEHLP_LINE64 line = {};
			line.SizeOfStruct = sizeof(IMAGEHLP_LINE64);

			std::ostringstream oss;
			oss << "StackTrace:\n";
			for (DWORD64 i = 0; i < m_size; ++i)
			{
				oss << i + 1;
				DWORD64 address = m_frames[i];
				if (SymFromAddr(process, address, &symDisplacement, symbol))
				{
					oss << " in " << symbol->Name;
					if (SymGetLineFromAddr64(process, address, &lineDisplacement, &line))
					{
						oss << " at " << line.FileName << " : " << line.LineNumber;
					}
				}
				else
				{
					std::ios oldState(nullptr);
					oldState.copyfmt(oss);
					oss << " 0x" << std::hex << std::uppercase
#ifdef BASE_64BIT
						<< std::setw(16) << std::setfill('0')
#else
						<< std::setw(8) << std::setfill('0')
#endif
						<< address;
					oss.copyfmt(oldState);
				}
				oss << '\n';
			}

			SymCleanup(process);

			BASE_LOG(fatal) << oss.str() << std::flush;
			g_logger.flush();
		}
	}
}
