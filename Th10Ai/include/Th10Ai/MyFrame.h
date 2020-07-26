#pragma once

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

		Logger m_logger;
		unique_ptr<Th10Ai> m_th10Ai;

		wxDECLARE_EVENT_TABLE();
	};
}
