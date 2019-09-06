#include "TH10AI/Common.h"

#include <libTH10AI/HookMain.h>

int APIENTRY wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int)
{
	th::HookMain();
	return 0;
}
