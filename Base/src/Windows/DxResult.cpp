#include "Windows/DxResult.h"

#include <iomanip>

#include "Windows/Apis.h"

namespace win
{
	DxResult::DxResult(HRESULT result) :
		m_result(result),
		m_custom(false)
	{
	}

	DxResult::DxResult(HRESULT result, const char* str) :
		Exception(str),
		m_result(result),
		m_custom(true)
	{

	}

	DxResult::DxResult(HRESULT result, const std::string& str) :
		Exception(str),
		m_result(result),
		m_custom(true)
	{

	}

	void DxResult::print(std::ostream& os) const
	{
		std::ios oldState(nullptr);
		oldState.copyfmt(os);
		os << "[0x" << std::hex << std::uppercase << std::setw(8)
			<< std::setfill('0') << m_result << ']';
		os.copyfmt(oldState);

		if (m_custom)
			os << what() << '\n';
		else
			os << Apis::GetErrorDesc(m_result);

		printSourceLocation(os);
		printStackTrace(os);
	}
}
