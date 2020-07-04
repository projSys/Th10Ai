#include "Th10Ai/Common.h"
#include "Th10Ai/MainWindow.h"

namespace th
{
	wxBEGIN_EVENT_TABLE(MainWindow, wxWindow)
	wxEND_EVENT_TABLE()

	MainWindow::MainWindow(wxWindow* parent) :
		wxWindow(parent, wxID_ANY)
	{
		SetClientSize(384, 448);
		SetMinClientSize(wxSize(384, 448));
		SetBackgroundColour(*wxWHITE);
	}

	MainWindow::~MainWindow()
	{
	}
}
