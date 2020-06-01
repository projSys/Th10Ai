#pragma once

namespace th
{
	class MyDetours
	{
	public:
		MyDetours();
		~MyDetours();

		// GetCurrentThread()
		void updateThread(HANDLE thread);

		void attach(PVOID* pointer, PVOID detour);
		void detach(PVOID* pointer, PVOID detour);
	};
}
