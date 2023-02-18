#include "Th10Ai/MyApp.h"

#include "Th10Ai/MyFrame.h"

wxIMPLEMENT_APP(th::MyApp);

namespace th
{
	bool MyApp::OnInit()
	{
		MyFrame* frame = new MyFrame();
		frame->Show();
		frame->focus();
		return true;
	}
}
