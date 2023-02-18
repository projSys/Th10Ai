#include "Th10Ai/MyFrame.h"

#include <Base/Exception/Catcher.h>
#include <Base/Windows/Apis.h>

namespace th
{
	enum
	{
		myID_START = wxID_HIGHEST + 1,
		myID_STOP
	};

	wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
		EVT_CLOSE(MyFrame::onClose)
		EVT_BUTTON(myID_START, MyFrame::onStart)
		EVT_BUTTON(myID_STOP, MyFrame::onStop)
	wxEND_EVENT_TABLE()

	MyFrame::MyFrame() :
		wxFrame(nullptr, wxID_ANY, wxT("Th10Ai")),
		m_statusWindow(nullptr)
	{
		SetWindowStyle(wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX));
		SetClientSize(640, 480);
		RECT rect = {};
		::GetWindowRect(GetHandle(), &rect);
		int32_t windowWidth = rect.right - rect.left;
		int32_t windowHeight = rect.bottom - rect.top;
		int32_t screenWidth = ::GetSystemMetrics(SM_CXSCREEN);
		int32_t screenHeight = ::GetSystemMetrics(SM_CYSCREEN);
		int32_t x = (screenWidth / 2);
		int32_t y = (screenHeight - windowHeight) / 2;
		SetPosition(wxPoint(x, y));

		wxPanel* panel = new wxPanel(this, wxID_ANY);
		wxBoxSizer* verSizer = new wxBoxSizer(wxVERTICAL);
		wxBoxSizer* horSizer = new wxBoxSizer(wxHORIZONTAL);
		m_statusWindow = new MyWindow(panel);
		wxGridSizer* gridSizer = new wxGridSizer(1, 2, 0, 0);
		wxButton* startBtn = new wxButton(panel, myID_START, wxT("StartAI"));
		wxButton* stopBtn = new wxButton(panel, myID_STOP, wxT("StopAI"));

		panel->SetSizer(verSizer);

		verSizer->AddSpacer(16);
		verSizer->Add(horSizer, wxSizerFlags());
		verSizer->AddSpacer(16);

		horSizer->AddSpacer(32);
		horSizer->Add(m_statusWindow, wxSizerFlags());
		horSizer->AddSpacer(16);
		horSizer->Add(gridSizer, wxSizerFlags());
		horSizer->AddSpacer(16);

		gridSizer->Add(startBtn, wxSizerFlags());
		gridSizer->Add(stopBtn, wxSizerFlags());

		try
		{
			fs::path dir = Apis::GetModuleDir();
			fs::path logPath = dir / L"Th10Ai.log";
			Log::Initialize(logPath);
		}
		catch (...)
		{
			MessageBoxW(nullptr, L"Log初始化失败。", L"Th10Ai", MB_OK);
			throw;
		}

		try
		{
			m_th10Ai = std::make_unique<Th10Ai>(this);
		}
		catch (...)
		{
			LOG_FATAL() << Catcher() << std::endl;
			Log::Flush();
		}
	}

	MyFrame::~MyFrame()
	{
	}

	void MyFrame::focus()
	{
		m_th10Ai->focus();
	}

	void MyFrame::onClose(wxCloseEvent& event)
	{
		m_th10Ai = nullptr;

		event.Skip();
	}

	void MyFrame::onStart(wxCommandEvent& event)
	{
		m_th10Ai->start();
		m_th10Ai->focus();
	}

	void MyFrame::onStop(wxCommandEvent& event)
	{
		m_th10Ai->stop();
		m_th10Ai->focus();
	}

	void MyFrame::onStatusUpdate(const SharedStatus& status)
	{
		m_statusWindow->onStatusUpdate(status);
	}
}
