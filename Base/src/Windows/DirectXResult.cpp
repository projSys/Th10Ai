#include "Windows/Common.h"
#include "Windows/DirectXResult.h"

#include <iomanip>

#include "Windows/Util.h"

namespace win
{
	// DX的错误信息有问题
	DirectXResult::DirectXResult(HRESULT result,
		const char* func, const char* file, uint_t line) :
		Exception(Util::GetErrorMessage(result), func, file, line, 4),
		m_result(result)
	{
	}

	void DirectXResult::print(std::ostream& os) const
	{
		std::ios oldState(nullptr);
		oldState.copyfmt(os);
		os << "[0x" << std::hex << std::uppercase << std::setw(8)
			<< std::setfill('0') << m_result << ']';
		os.copyfmt(oldState);
		Exception::print(os);
	}

	HRESULT DirectXResult::getResult() const
	{
		return m_result;
	}
}
