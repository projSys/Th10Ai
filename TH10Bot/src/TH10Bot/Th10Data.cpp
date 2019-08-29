#include "TH10Bot/Common.h"
#include "TH10Bot/Th10Data.h"

namespace th
{
	Th10Data::Th10Data()
	{
		items.reserve(200);
		enemies.reserve(200);
		bullets.reserve(2000);
		lasers.reserve(200);
	}
}
