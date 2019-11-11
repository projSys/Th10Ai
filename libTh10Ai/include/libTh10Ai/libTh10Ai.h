#pragma once

#include <memory>

#include "libTh10Ai/DllInject/DllInject.h"

namespace th
{
#ifdef LIBTH10AI_EXPORTS
#define DLL_API __declspec(dllexport)
#else
#define DLL_API __declspec(dllimport)
#endif

	extern "C" DLL_API void WINAPI Th10AiMain();

	class Th10Ai;

	class libTh10Ai :
		public DllInject
	{
	private:
		virtual bool onAttach();
		virtual void onDetach();

		std::shared_ptr<Th10Ai> m_th10Ai;
	};

	extern libTh10Ai g_libTh10Ai;
}
