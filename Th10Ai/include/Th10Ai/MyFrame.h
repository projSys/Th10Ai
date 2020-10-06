#pragma once

#include "Th10Ai/Common.h"

#include <memory>
#include <wx/wx.h>

#include "Th10Ai/Th10Ai.h"

namespace th
{
	class MyFrame :
		public wxFrame
	{
	public:
		MyFrame();
		virtual ~MyFrame();

	private:
		void onClose(wxCloseEvent& event);

		std::unique_ptr<Th10Ai> m_th10Ai;

		wxDECLARE_EVENT_TABLE();
	};
}
