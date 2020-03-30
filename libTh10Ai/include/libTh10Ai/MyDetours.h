#pragma once

namespace th
{
	class MyDetours
	{
	public:
		MyDetours();
		~MyDetours();

		void attach(PVOID* pointer, PVOID detour);
		void detach(PVOID* pointer, PVOID detour);
	};
}
