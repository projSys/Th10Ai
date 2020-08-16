#include "Th10Ai/Common.h"
#include "Th10Ai/Scene.h"

#include <cmath>

namespace th
{
	const Sizef Scene::SIZE = { 384.0f, 448.0f };
	const Sizef Scene::OFFSET = { 32.0f, 16.0f };
	const Sizef Scene::ORIGIN_POINT_OFFSET = { 192.0f, 0.0f };

	bool Scene::IsInScene(const Pointf& pos)
	{
		return pos.x >= -192.0f && pos.x <= 192.0f && pos.y >= 0.0f && pos.y <= 448.0f;
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

	Pointi Scene::ToWindowPos(const Pointf& scenePos)
	{
		return Pointi(static_cast<int_t>(std::round(scenePos.x + ORIGIN_POINT_OFFSET.width + OFFSET.width)),
			static_cast<int_t>(std::round(scenePos.y + ORIGIN_POINT_OFFSET.height + OFFSET.height)));
	}

	Pointf Scene::ToScenePos(const Pointi& windowPos)
	{
		return Pointf(static_cast<float_t>(windowPos.x) - OFFSET.width - ORIGIN_POINT_OFFSET.width,
			static_cast<float_t>(windowPos.y) - OFFSET.height - ORIGIN_POINT_OFFSET.height);
	}

	Scene::Scene() :
		m_cell(0.0f, 224.0f, 384.0f, 448.0f)
	{
	}

	void Scene::split(int_t times)
	{
		m_cell.split(times);
	}

	void Scene::clearAll()
	{
		m_cell.clearAll();
	}

	void Scene::splitEnemies(const std::vector<Enemy>& enemies)
	{
		m_cell.splitEnemies(enemies);
	}

	void Scene::splitBullets(const std::vector<Bullet>& bullets)
	{
		m_cell.splitBullets(bullets);
	}

	void Scene::splitLasers(const std::vector<Laser>& lasers)
	{
		m_cell.splitLasers(lasers);
	}

	CellCollideResult Scene::collideAll(const Player& player, float_t frame) const
	{
		return m_cell.collideAll(player, frame);
	}
}
