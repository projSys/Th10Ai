#include "Base/Windows/StackTraceImpl.h"

#include <DbgHelp.h>
#include <ostream>
#include <iomanip>

#pragma comment(lib, "DbgHelp.lib")

namespace base
{
	namespace win
	{
		StackTraceImpl::StackTraceImpl(DWORD skips) :
			m_frames{},
			m_size(0)
		{
#ifdef _DEBUG
			m_size = RtlCaptureStackBackTrace(skips, BUFFER_SIZE, m_frames, nullptr);
#else
			m_size = RtlCaptureStackBackTrace(0, BUFFER_SIZE, m_frames, nullptr);
#endif
		}

		void StackTraceImpl::toStream(std::ostream& os) const
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

			os << "StackTrace:\n";
			for (WORD i = 0; i < m_size; ++i)
			{
				os << i + 1;
				DWORD64 address = reinterpret_cast<DWORD64>(m_frames[i]);
				if (SymFromAddr(process, address, &symDisplacement, symbol))
				{
					os << " in " << symbol->Name;
					if (SymGetLineFromAddr64(process, address, &lineDisplacement, &line))
					{
						os << " at " << line.FileName << " : " << line.LineNumber;
					}
				}
				else
				{
					std::ios oldState(nullptr);
					oldState.copyfmt(os);
					os << " 0x" << std::hex << std::uppercase
#ifdef BASE_64BIT
						<< std::setw(16) << std::setfill('0')
#else
						<< std::setw(8) << std::setfill('0')
#endif
						<< address;
					os.copyfmt(oldState);
				}
				os << '\n';
			}

			SymCleanup(process);
		}
	}
}
