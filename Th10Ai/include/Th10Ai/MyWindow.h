#pragma once

#include "Th10Ai/Common.h"

#include <memory>
#include <mutex>
#include <wx/wx.h>

#include "Th10Ai/Status.h"

namespace th
{
	class MyWindow :
		public wxWindow
	{
	public:
		explicit MyWindow(wxWindow* parent);
		virtual ~MyWindow();

		void onStatusUpdate(const Status& status);

	private:
		void onPaint(wxPaintEvent& event);
		void onEraseBackground(wxEraseEvent& event);

		wxSize m_size;
		wxBitmap m_buffer;

		std::mutex m_statusMutex;
		bool m_statusUpdated;
		std::unique_ptr<Status> m_writableStatus;
		std::unique_ptr<Status> m_swappableStatus;
		std::unique_ptr<Status> m_readableStatus;

		wxDECLARE_EVENT_TABLE();
	};
}
