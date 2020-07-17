#pragma once

#include <memory>
#include <wx/wx.h>

#include "Th10Ai/Th10Context.h"

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
		unique_ptr<Th10Context> m_th10Context;

		wxDECLARE_EVENT_TABLE();
	};
}
