#include "TH10Bot/Common.h"
#include "TH10Bot/Scene.h"

#include <cmath>

namespace th
{
	const Pointf Scene::SCENE_OFFSET = { 24.0f, 16.0f };
	const Pointf Scene::ORIGIN_POINT_OFFSET = { 200.0f, 0.0f };

	Pointi Scene::ToWindowPos(const Pointf& scenePos)
	{
		return Pointi(static_cast<int_t>(std::round(scenePos.x + ORIGIN_POINT_OFFSET.x + SCENE_OFFSET.x)),
			static_cast<int_t>(std::round(scenePos.y + ORIGIN_POINT_OFFSET.y + SCENE_OFFSET.y)));
	}

	Pointf Scene::ToScenePos(const Pointi& windowPos)
	{
		return Pointf(static_cast<float_t>(windowPos.x) - SCENE_OFFSET.x - ORIGIN_POINT_OFFSET.x,
			static_cast<float_t>(windowPos.y) - SCENE_OFFSET.y - ORIGIN_POINT_OFFSET.y);
	}

	bool Scene::IsInPlayerArea(const Pointf& pos)
	{
		return pos.x >= -184.0f && pos.x <= 184.0f && pos.y >= 32.0f && pos.y <= 432.0f;
	}

	Pointf Scene::FixPlayerPos(const Pointf& pos)
	{
		Pointf ret = pos;
		if (ret.x < -184.0f)
			ret.x = -184.0f;
		if (ret.x > 184.0f)
			ret.x = 184.0f;
		if (ret.y < 32.0f)
			ret.y = 32.0f;
		if (ret.y > 432.0f)
			ret.y = 432.0f;
		return ret;
	}

	Scene::Scene()
	{
		m_area.x = 0.0f;
		m_area.y = 448.0f / 2.0f;
		m_area.dx = 0.0f;
		m_area.dy = 0.0f;
		m_area.width = 384.0f;
		m_area.height = 448.0f;
	}

	void Scene::split(int_t times)
	{
		m_area.split(times);
	}

	void Scene::clear()
	{
		m_area.clear();
	}

	void Scene::splitEnemies(const std::vector<Enemy>& enemies)
	{
		m_area.splitEnemies(enemies);
	}

	void Scene::splitBullets(const std::vector<Bullet>& bullets)
	{
		m_area.splitBullets(bullets);
	}

	void Scene::splitLasers(const std::vector<Laser>& lasers)
	{
		m_area.splitLasers(lasers);
	}

	void Scene::renderTo(cv::Mat& buffer, const Player& player)
	{
		m_area.renderTo(buffer, player);
	}
}
