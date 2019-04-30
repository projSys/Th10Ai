#include "TH10Bot/Common.h"
#include "TH10Bot/Scene.h"

#include <cmath>

namespace th
{
	Pointi Scene::ToWindowPos(const Pointf& scenePos)
	{
		return Pointi(static_cast<int_t>(std::round(SCENE_POS.x + ORIGIN_POINT_POS.x + scenePos.x)),
			static_cast<int_t>(std::round(SCENE_POS.y + ORIGIN_POINT_POS.y + scenePos.y)));
	}

	Pointf Scene::ToScenePos(const Pointi& windowPos)
	{
		return Pointf(static_cast<float_t>(windowPos.x) - SCENE_POS.x - ORIGIN_POINT_POS.x,
			static_cast<float_t>(windowPos.y) - SCENE_POS.y - ORIGIN_POINT_POS.y);
	}

	bool Scene::IsInScene(const Pointf& pos)
	{
		return pos.x >= -184.0f && pos.x <= 184.0f && pos.y >= 32.0f && pos.y <= 432.0f;
	}

	Pointf Scene::FixPos(const Pointf& pos)
	{
		Pointf fixed = pos;
		if (fixed.x < -184.0f)
			fixed.x = -184.0f;
		if (fixed.x > 184.0f)
			fixed.x = 184.0f;
		if (fixed.y < 32.0f)
			fixed.y = 32.0f;
		if (fixed.y > 432.0f)
			fixed.y = 432.0f;
		return fixed;
	}
}
