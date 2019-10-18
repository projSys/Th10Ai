#pragma once

#include <memory>
#include <Base/Singleton.h>

void WINAPI Th10AiMain();

namespace th
{
	class Th10Ai;

	class libTh10Ai :
		public Singleton<libTh10Ai>
	{
	public:
		libTh10Ai();

		bool attach();
		void detach();
		void wait();
		void notify();

	private:
		static LRESULT CALLBACK CallWndProc(int code, WPARAM wParam, LPARAM lParam);
		LRESULT callWndProc(int code, WPARAM wParam, LPARAM lParam);
		void onAttach();
		void onDetach();
		void onDestroy();

		std::shared_ptr<Th10Ai> m_th10Ai;
	};
}
