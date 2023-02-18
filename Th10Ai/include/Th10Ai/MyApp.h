#pragma once

#include "Th10Ai/Common.h"

#include <wx/wx.h>

namespace th
{
	class MyApp :
		public wxApp
	{
	public:
		virtual bool OnInit() override;
	};
}
