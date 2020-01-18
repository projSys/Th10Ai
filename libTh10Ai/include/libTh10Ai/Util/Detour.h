#pragma once

namespace th
{
	class Detour
	{
	public:
		Detour();
		~Detour();

		void attach(PVOID* pointer, PVOID detour);
		void detach(PVOID* pointer, PVOID detour);
	};
}
