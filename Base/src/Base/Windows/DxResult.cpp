#include "Base/Windows/DxResult.h"

#include <iomanip>

#include "Base/Windows/Apis.h"

namespace base
{
	namespace win
	{
		DxResult::DxResult(HRESULT result) :
			Exception(1),
			m_result(result),
			m_customized(false)
		{
		}

		DxResult::DxResult(HRESULT result, const char* str) :
			Exception(str, 1),
			m_result(result),
			m_customized(true)
		{

		}

		DxResult::DxResult(HRESULT result, const std::string& str) :
			Exception(str, 1),
			m_result(result),
			m_customized(true)
		{

		}

		void DxResult::printTo(std::ostream& os) const
		{
			std::ios oldState(nullptr);
			oldState.copyfmt(os);
			os << "[0x" << std::hex << std::uppercase << std::setw(8)
				<< std::setfill('0') << m_result << ']';
			os.copyfmt(oldState);

			if (m_customized)
				os << what() << '\n';
			else
				os << Apis::GetErrorDesc(m_result);

			m_sourceLocation.printTo(os);
			m_stackTrace.printTo(os);
		}
	}
}
