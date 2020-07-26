#include "Th10Ai/Common.h"
#include "Th10Ai/MyFrame.h"

#include "Th10Ai/MyWindow.h"

namespace th
{
	wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
		EVT_CLOSE(MyFrame::onClose)
	wxEND_EVENT_TABLE()

	MyFrame::MyFrame() :
		wxFrame(nullptr, wxID_ANY, wxT("Th10Ai"))
	{
		SetWindowStyle(wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX));
		SetClientSize(640, 480);

		wxPanel* panel = new wxPanel(this, wxID_ANY);

		MyWindow* window = new MyWindow(panel);

		wxBoxSizer* hSizer = new wxBoxSizer(wxHORIZONTAL);
		hSizer->AddSpacer(32);
		hSizer->Add(window, wxSizerFlags().CentreVertical());
		hSizer->AddSpacer(32);

		wxBoxSizer* vSizer = new wxBoxSizer(wxVERTICAL);
		vSizer->Add(new wxButton(panel, wxID_OK, "OK"), wxSizerFlags());
		vSizer->Add(new wxButton(panel, wxID_CANCEL, "Cancel"), wxSizerFlags());
		hSizer->Add(vSizer, wxSizerFlags());

		panel->SetSizer(hSizer);

		string logName = Apis::GetModuleDir() + "/Th10Ai_%N.log";
		m_logger.addFileLog(logName);

		m_th10Ai = make_unique<Th10Ai>(window);
	}

	MyFrame::~MyFrame()
	{
	}

	void MyFrame::onClose(wxCloseEvent& event)
	{
		m_th10Ai = nullptr;

		event.Skip();
	}
}
