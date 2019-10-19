#pragma once

#include <memory>

void WINAPI Th10AiMain();

namespace th
{
	class Th10Ai;

	class libTh10Ai
	{
	public:
		static void Main();
		static void Exit();

	private:
		static LRESULT CALLBACK CallWndProc(int code, WPARAM wParam, LPARAM lParam);
		static void OnAttach();
		static void OnDetach();
		static void OnDestroy();

		static std::shared_ptr<Th10Ai> s_th10Ai;
	};
}
