#include "Th10Hook/Scene.h"

#include <math.h>
#include <cmath>

namespace th
{
	bool Scene::IsInScene(const vec2& pos)
	{
		return pos.x >= _F(-192.0) && pos.x <= _F(191.0)
			&& pos.y >= _F(0.0) && pos.y <= _F(447.0);
	}

	bool Scene::IsInPlayerRegion(const vec2& pos)
	{
		return pos.x >= _F(-184.0) && pos.x <= _F(183.0)
			&& pos.y >= _F(32.0) && pos.y <= _F(431.0);
	}

	vec2 Scene::ToWindowPos(const vec2& scenePos)
	{
		return vec2(std::round(scenePos.x + ORIGIN_POINT_OFFSET.x + OFFSET.x),
			std::round(scenePos.y + ORIGIN_POINT_OFFSET.y + OFFSET.y));
	}

	vec2 Scene::ToScenePos(const vec2& windowPos)
	{
		return vec2(windowPos.x - OFFSET.x - ORIGIN_POINT_OFFSET.x,
			windowPos.y - OFFSET.y - ORIGIN_POINT_OFFSET.y);
	}

	Scene::Scene() :
		m_region(vec2(_F(0.0), _F(224.0)), SIZE)
	{
		m_region.split(4);
	}

	void Scene::clearAll()
	{
		m_region.clearAll();
	}

	void Scene::splitEnemies(const std::vector<Enemy>& enemies)
	{
		m_region.splitEnemies(enemies);
	}

	void Scene::splitBullets(const std::vector<Bullet>& bullets)
	{
		m_region.splitBullets(bullets);
	}

	void Scene::splitLasers(const std::vector<Laser>& lasers)
	{
		m_region.splitLasers(lasers);
	}

	RegionCollideResult Scene::collideAll(const Player& player, float_t frame) const
	{
		return m_region.collideAll(player, frame);
	}

	RegionCollideResult Scene::collideAll(const Player& player, float_t frame, const Bullet& target) const
	{
		return m_region.collideAll(player, frame, target);
	}

#if RENDER
	void Scene::render(cv::Mat& mat, const Player& player)
	{
		m_region.render(mat, player);
	}
#endif
}
