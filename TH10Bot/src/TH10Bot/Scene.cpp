#include "TH10Bot/Common.h"
#include "TH10Bot/Scene.h"

namespace th
{
	Pointi Scene::ToWindowPos(const Pointf& gamePos)
	{
		return Pointi(static_cast<Pointi::value_t>(SCENE_POS.x + GAME_OFFSET.x + gamePos.x + 0.5f),
			static_cast<Pointi::value_t>(SCENE_POS.y + GAME_OFFSET.y + gamePos.y + 0.5f));
	}

	Pointf Scene::ToGamePos(const Pointi& windowPos)
	{
		return Pointf(static_cast<Pointf::value_t>(windowPos.x) - SCENE_POS.x - GAME_OFFSET.x,
			static_cast<Pointf::value_t>(windowPos.y) - SCENE_POS.y - GAME_OFFSET.y);
	}

	bool Scene::IsInScene(const Pointf& pos)
	{
		return pos.x > -192.0f && pos.x < 192.0f && pos.y > 0.0f && pos.y < 448.0f;
	}
}
