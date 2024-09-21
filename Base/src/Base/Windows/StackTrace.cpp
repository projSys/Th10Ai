#include "Base/Windows/StackTrace.h"

#include <DbgHelp.h>
#include <mutex>
#include <ostream>
#include <iomanip>

#include "Base/ScopeGuard.h"

#pragma comment(lib, "DbgHelp.lib")

#if defined(_MSC_VER) && _MSC_VER >= 1900
extern "C" void** __cdecl __current_exception_context();
#endif

namespace base
{
	namespace win
	{
		StackTrace StackTrace::Current(uint_t skip)
		{
			CONTEXT context = {};
			RtlCaptureContext(&context);
			return StackTrace(context, skip);
		}

		StackTrace StackTrace::FromCurrentException(uint_t skip)
		{
			PCONTEXT* context = reinterpret_cast<PCONTEXT*>(__current_exception_context());
			if (context != nullptr && *context != nullptr)
				return StackTrace(**context, skip);
			else
				return StackTrace();
		}

		StackTrace::StackTrace() :
			m_context{},
			m_skip(0)
		{
		}

		StackTrace::StackTrace(const CONTEXT& context, uint_t skip) :
			m_context{ context },
			m_skip(skip)
		{
		}

		void StackTrace::toStream(std::ostream& out) const
		{
			static std::mutex mutex;
			std::lock_guard<std::mutex> lock(mutex);

			HANDLE process = GetCurrentProcess();
			HANDLE thread = GetCurrentThread();

			SymSetOptions(SYMOPT_LOAD_LINES | SYMOPT_UNDNAME);

			if (!SymInitialize(process, nullptr, TRUE))
			{
				out << "SymInitialize() failed: " << GetLastError() << '\n';
				return;
			}
			ON_SCOPE_EXIT([&]()
				{
					if (!SymCleanup(process))
						out << "SymCleanup() failed: " << GetLastError() << '\n';
				});

			DWORD machineType = 0;
			STACKFRAME64 frame = {};
#ifdef _M_IX86
			machineType = IMAGE_FILE_MACHINE_I386;
			frame.AddrPC.Offset = m_context.Eip;
			frame.AddrPC.Mode = AddrModeFlat;
			frame.AddrFrame.Offset = m_context.Ebp;
			frame.AddrFrame.Mode = AddrModeFlat;
			frame.AddrStack.Offset = m_context.Esp;
			frame.AddrStack.Mode = AddrModeFlat;
#elif _M_X64
			machineType = IMAGE_FILE_MACHINE_AMD64;
			frame.AddrPC.Offset = m_context.Rip;
			frame.AddrPC.Mode = AddrModeFlat;
			frame.AddrFrame.Offset = m_context.Rsp;
			frame.AddrFrame.Mode = AddrModeFlat;
			frame.AddrStack.Offset = m_context.Rsp;
			frame.AddrStack.Mode = AddrModeFlat;
#elif _M_IA64
			machineType = IMAGE_FILE_MACHINE_IA64;
			frame.AddrPC.Offset = m_context.StIIP;
			frame.AddrPC.Mode = AddrModeFlat;
			frame.AddrFrame.Offset = m_context.IntSp;
			frame.AddrFrame.Mode = AddrModeFlat;
			frame.AddrBStore.Offset = m_context.RsBSP;
			frame.AddrBStore.Mode = AddrModeFlat;
			frame.AddrStack.Offset = m_context.IntSp;
			frame.AddrStack.Mode = AddrModeFlat;
#elif _M_ARM64
			machineType = IMAGE_FILE_MACHINE_ARM64;
			frame.AddrPC.Offset = m_context.Pc;
			frame.AddrPC.Mode = AddrModeFlat;
			frame.AddrFrame.Offset = m_context.Fp;
			frame.AddrFrame.Mode = AddrModeFlat;
			frame.AddrStack.Offset = m_context.Sp;
			frame.AddrStack.Mode = AddrModeFlat;
#else
#error "Platform not supported!"
#endif

			// context会被修改
			CONTEXT context = m_context;
			uint_t skip = m_skip;

			BYTE buffer[sizeof(IMAGEHLP_SYMBOL64) + sizeof(CHAR) * (MAX_SYM_NAME + 1)] = {};
			PIMAGEHLP_SYMBOL64 symbol = reinterpret_cast<PIMAGEHLP_SYMBOL64>(buffer);
			symbol->SizeOfStruct = sizeof(IMAGEHLP_SYMBOL64);
			symbol->MaxNameLength = MAX_SYM_NAME;
			DWORD64 symDisp = 0;

			IMAGEHLP_LINE64 line = {};
			line.SizeOfStruct = sizeof(IMAGEHLP_LINE64);
			DWORD lineDisp = 0;

			uint_t i = 0;
			while (true)
			{
				if (!StackWalk64(machineType, process, thread, &frame, &context,
					nullptr, SymFunctionTableAccess64, SymGetModuleBase64, nullptr))
					break;
				if (frame.AddrPC.Offset == 0)
					break;

				if (skip != 0)
				{
					skip--;
					continue;
				}

				DWORD64 address = frame.AddrPC.Offset;
				out << i++ << "# ";
				if (SymGetSymFromAddr64(process, address, &symDisp, symbol))
				{
					out << symbol->Name;
					if (SymGetLineFromAddr64(process, address, &lineDisp, &line))
						out << " at " << line.FileName << ":" << line.LineNumber;
				}
				else
				{
					std::ios oldState(nullptr);
					oldState.copyfmt(out);
					out << "0x" << std::hex << std::uppercase << std::setfill('0');
#ifdef BASE_64BIT
					out << std::setw(16) << address;
#else
					out << std::setw(8) << address;
#endif
					out.copyfmt(oldState);
				}
				out << '\n';

				if (frame.AddrReturn.Offset == 0)
					break;
			}
		}
	}
}
