#pragma once

#include "Th10Ai/Common.h"

#include <memory>
#include <wx/wx.h>

#include "Th10Ai/MyWindow.h"
#include "Th10Ai/Th10Ai.h"
#include "Th10Ai/Listener.h"

namespace th
{
	class MyFrame :
		public wxFrame,
		public Listener
	{
	public:
		MyFrame();
		virtual ~MyFrame();

		void focus();

	private:
		void onClose(wxCloseEvent& event);
		void onStart(wxCommandEvent& event);
		void onStop(wxCommandEvent& event);

		virtual void onStatusUpdate(const SharedStatus& status) override;

		MyWindow* m_statusWindow;

		std::unique_ptr<Th10Ai> m_th10Ai;

		wxDECLARE_EVENT_TABLE();
	};
}
