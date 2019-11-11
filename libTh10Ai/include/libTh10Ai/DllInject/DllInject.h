#pragma once

#include <Base/Singleton.h>

namespace th
{
	class DllInject :
		public Singleton<DllInject>
	{
	public:
		DllInject();
		virtual ~DllInject() = default;

		void main(LPCTSTR className, LPCTSTR windowName);
		void exit();

	protected:
		static LRESULT CALLBACK CallWndProc(int code, WPARAM wParam, LPARAM lParam);
		LRESULT callWndProc(int code, WPARAM wParam, LPARAM lParam);
		virtual bool onAttach();
		virtual void onDetach();
	};
}
