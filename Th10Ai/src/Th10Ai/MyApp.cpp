#include "Th10Ai/Common.h"
#include "Th10Ai/MyApp.h"

#include "Th10Ai/MyFrame.h"

wxIMPLEMENT_APP(th::MyApp);

namespace th
{
	bool MyApp::OnInit()
	{
		if (!wxApp::OnInit())
			return false;

		MyFrame* frame = new MyFrame();
		frame->Show(true);

		return true;
	}
}
