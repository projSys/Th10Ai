#include "Th10Ai/MyApp.h"

#include "Th10Ai/MyFrame.h"

wxIMPLEMENT_APP(th::MyApp);
//wxIMPLEMENT_APP_NO_MAIN(th::MyApp);

namespace th
{
	bool MyApp::OnInit()
	{
		if (!wxApp::OnInit())
			return false;

		MyFrame* frame = new MyFrame();
		frame->Show();
		frame->focus();
		return true;
	}
}
