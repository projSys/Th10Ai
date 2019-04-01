#include "TH10Bot/Common.h"
#include "TH10Bot/Scene.h"

#include <cmath>

namespace th
{
	Pointi Scene::ToWindowPos(const Pointf& scenePos)
	{
		return Pointi(static_cast<Pointi::value_t>(std::round(SCENE_POS.x + POS_OFFSET.x + scenePos.x)),
			static_cast<Pointi::value_t>(std::round(SCENE_POS.y + POS_OFFSET.y + scenePos.y)));
	}

	Pointf Scene::ToScenePos(const Pointi& windowPos)
	{
		return Pointf(static_cast<Pointf::value_t>(windowPos.x) - SCENE_POS.x - POS_OFFSET.x,
			static_cast<Pointf::value_t>(windowPos.y) - SCENE_POS.y - POS_OFFSET.y);
	}

	bool Scene::IsInScene(const Pointf& pos)
	{
		return pos.x > -184.0f && pos.x < 184.0f && pos.y > 32.0f && pos.y < 432.0f;
	}
}
