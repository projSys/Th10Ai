#pragma once

#include <wx/wx.h>

namespace th
{
	class MainWindow :
		public wxWindow
	{
	public:
		MainWindow(wxWindow* parent);
		virtual ~MainWindow();

	private:

		wxDECLARE_EVENT_TABLE();
	};
}
