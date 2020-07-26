#pragma once

#include <wx/wx.h>

#include "Th10Ai/Status.h"

namespace th
{
	class MyWindow :
		public wxWindow
	{
	public:
		MyWindow(wxWindow* parent);
		virtual ~MyWindow();

		void update(Status_t& status);

	private:
		void onPaint(wxPaintEvent& event);
		void onEraseBackground(wxEraseEvent& event);

		wxSize m_size;
		wxBitmap m_buffer;

		Status_t m_status;

		wxDECLARE_EVENT_TABLE();
	};
}
