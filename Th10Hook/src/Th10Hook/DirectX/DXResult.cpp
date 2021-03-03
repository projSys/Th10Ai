#include "Th10Hook/DirectX/DXResult.h"

#include <iomanip>
#include <Base/Windows/Apis.h>

namespace th
{
	DXResult::DXResult(HRESULT result, const char* str) :
		Exception(str),
		m_result(result),
		m_customized(!String::IsEmpty(str))
	{
	}

	DXResult::DXResult(HRESULT result, const std::string& str) :
		Exception(str),
		m_result(result),
		m_customized(!str.empty())
	{
	}

	void DXResult::printTo(std::ostream& os) const
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
