#pragma once

#include "Th10Hook/Common.h"

namespace th
{
	class Console
	{
	public:
		Console();
		~Console();

	private:
		FILE* m_newStdin;
		FILE* m_newStdout;
		FILE* m_newStderr;
	};
}
