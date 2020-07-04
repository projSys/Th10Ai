#include "Th10Ai/Common.h"
#include "Th10Ai/MyFrame.h"

#include <sstream>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/file.hpp>

#include "Th10Ai/MainWindow.h"
#include "Th10Ai/DllInject.h"

namespace th
{
	using namespace boost::interprocess;
	using namespace boost::posix_time;
	namespace bl = boost::log;

	wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
		EVT_CLOSE(MyFrame::onClose)
	wxEND_EVENT_TABLE()

	MyFrame::MyFrame() :
		wxFrame(nullptr, wxID_ANY, wxT("Th10Ai")),
		m_data(nullptr)
	{
		SetWindowStyle(wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX));
		SetClientSize(640, 480);

		wxPanel* panel = new wxPanel(this, wxID_ANY);

		wxBoxSizer* hSizer = new wxBoxSizer(wxHORIZONTAL);
		hSizer->AddSpacer(32);
		hSizer->Add(new MainWindow(panel), wxSizerFlags().CentreVertical());
		hSizer->AddSpacer(32);

		wxBoxSizer* vSizer = new wxBoxSizer(wxVERTICAL);
		vSizer->Add(new wxButton(panel, wxID_OK, "OK"), wxSizerFlags());
		vSizer->Add(new wxButton(panel, wxID_CANCEL, "Cancel"), wxSizerFlags());
		hSizer->Add(vSizer, wxSizerFlags());

		panel->SetSizer(hSizer);

		try
		{
			string logName = Utils::GetModuleDir() + "/Th10Ai_%N.log";
			bl::add_file_log
			(
				bl::keywords::file_name = logName,
				bl::keywords::rotation_size = 10 * 1024 * 1024,
				bl::keywords::time_based_rotation = bl::sinks::file::rotation_at_time_point(0, 0, 0),
				bl::keywords::format = "%Message%"
			);

			HWND window = FindWindowW(L"BASE", nullptr);
			if (window == nullptr)
				BASE_THROW(Exception("Window not found."));

			DWORD processId = 0;
			GetWindowThreadProcessId(window, &processId);

			m_sharedMemory = managed_windows_shared_memory(create_only, "Th10HookMemory", 8 * 1024 * 1024);
			m_data = m_sharedMemory.construct<Th10HookData>("Th10HookData")();

			m_data->window = window;
			m_data->isHooked = false;
			m_data->isUnhook = false;

			string dllName = Utils::GetModuleDir() + "/Th10Hook.dll";
			DllInject::EnableDebugPrivilege();
			DllInject::Inject(processId, dllName);

			{
				scoped_lock<interprocess_mutex> lock(m_data->hookMutex);
				if (!m_data->isHooked)
				{
					ptime absTime = microsec_clock::universal_time() + milliseconds(3000);
					if (!m_data->hookCond.timed_wait(lock, absTime))
						BASE_THROW(Exception(u8"Th10Hook初始化失败，详细信息请查看Th10Hook.log。"));
				}
			}
			//{
			//	scoped_lock<interprocess_mutex> lock(m_data->unhookMutex);
			//	m_data->isUnhook = true;
			//	m_data->unhookCond.notify_one();
			//}

			BOOST_LOG_TRIVIAL(error) << "333333333333333333";
		}
		catch (...)
		{
			ostringstream oss;
			PrintException(oss);
			BOOST_LOG_TRIVIAL(error) << oss.str() << endl;
		}
	}

	MyFrame::~MyFrame()
	{
		if (m_data != nullptr)
			m_sharedMemory.destroy_ptr(m_data);
	}

	void MyFrame::onClose(wxCloseEvent& event)
	{
		event.Skip();
	}
}
