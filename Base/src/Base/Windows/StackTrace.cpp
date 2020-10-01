#include "Base/Windows/StackTrace.h"

#include <DbgHelp.h>

#pragma comment(lib, "DbgHelp.lib")

namespace base
{
	namespace win
	{
		StackTrace::StackTrace(uint_t framesToSkip)
		{
#ifdef _DEBUG
			m_frameCount = CaptureStackBackTrace(static_cast<DWORD>(framesToSkip + 1),
				FRAME_MAX_COUNT, m_frames, nullptr);
#else
			m_frameCount = CaptureStackBackTrace(0, FRAME_MAX_COUNT, m_frames, nullptr);
#endif
		}

		void StackTrace::printTo(std::ostream& os) const
		{
			//todo 线程同步、输出格式化
			HANDLE process = GetCurrentProcess();

			SymSetOptions(SYMOPT_LOAD_LINES);
			SymInitialize(process, nullptr, TRUE);

			byte_t buffer[sizeof(SYMBOL_INFO) + sizeof(CHAR) * (MAX_SYM_NAME + 1)] = {};
			PSYMBOL_INFO symbol = reinterpret_cast<PSYMBOL_INFO>(buffer);
			symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
			symbol->MaxNameLen = MAX_SYM_NAME;

			IMAGEHLP_LINE64 line = {};
			line.SizeOfStruct = sizeof(IMAGEHLP_LINE64);

			DWORD64 symDisplacement = 0;
			DWORD lineDisplacement = 0;

			os << "StackTrace:\n";
			for (WORD i = 0; i < m_frameCount; ++i)
			{
				DWORD64 address = reinterpret_cast<DWORD64>(m_frames[i]);

				SymFromAddr(process, address, &symDisplacement, symbol);
				SymGetLineFromAddr64(process, address, &lineDisplacement, &line);

				os << i + 1
					<< " in " << symbol->Name
					<< " at " << line.FileName
					<< " : " << line.LineNumber << '\n';
			}

			SymCleanup(process);
		}
	}
}
