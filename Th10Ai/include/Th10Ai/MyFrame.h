#pragma once

#include "Th10Ai/Common.h"

#include <memory>
#include <wx/wx.h>

#include "Th10Ai/MyWindow.h"
#include "Th10Ai/Th10Ai.h"
#include "Th10Ai/Th10AiListener.h"

namespace th
{
	class MyFrame :
		public wxFrame,
		public Th10AiListener
	{
	public:
		MyFrame();
		virtual ~MyFrame();

		void focus();

		virtual void onStatusUpdate(const Status& status) override;

	private:
		void onClose(wxCloseEvent& event);
		void onStart(wxCommandEvent& event);
		void onStop(wxCommandEvent& event);

		wxSize m_size;
		MyWindow* m_statusWindow;

		std::unique_ptr<Th10Ai> m_th10Ai;

		wxDECLARE_EVENT_TABLE();
	};
}
