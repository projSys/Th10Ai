#pragma once

namespace th
{
	class libDetours
	{
	public:
		libDetours();
		~libDetours();

		void attach(PVOID* pointer, PVOID detour);
		void detach(PVOID* pointer, PVOID detour);
	};
}
