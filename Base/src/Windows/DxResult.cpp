#include "Windows/Common.h"
#include "Windows/DxResult.h"

#include <iomanip>

#include "Windows/Utils.h"

namespace win
{
	DxResult::DxResult(HRESULT result) :
		m_result(result)
	{
	}

	void DxResult::print(ostream& os) const
	{
		ios oldState(nullptr);
		oldState.copyfmt(os);
		os << "[0x" << hex << uppercase << setw(8) << setfill('0') << m_result << ']';
		os.copyfmt(oldState);
		os << Utils::GetErrorDesc(m_result) << '\n';

		printSourceLocation(os);
		printStackTrace(os);
	}
}
