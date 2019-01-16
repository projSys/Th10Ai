#include "TH10Bot/Common.h"
#include "TH10Bot/Scene.h"

namespace th
{
	Pointi Scene::ToWindowPos(const Pointf& gamePos)
	{
		return Pointi(static_cast<Pointi::value_t>(SCENE_OFFSET.x + POS_OFFSET.x + gamePos.x + 0.5),
			static_cast<Pointi::value_t>(SCENE_OFFSET.y + POS_OFFSET.y + gamePos.y + 0.5));
	}

	Pointf Scene::ToGamePos(const Pointi& windowPos)
	{
		return Pointf(static_cast<Pointf::value_t>(windowPos.x) - SCENE_OFFSET.x - POS_OFFSET.x,
			static_cast<Pointf::value_t>(windowPos.y) - SCENE_OFFSET.y - POS_OFFSET.y);
	}
}
