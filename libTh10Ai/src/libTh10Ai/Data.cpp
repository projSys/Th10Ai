#include "libTh10Ai/Common.h"
#include "libTh10Ai/Data.h"

#include "libTh10Ai/Reader.h"
#include "libTh10Ai/Scene.h"

namespace th
{
	Data::Data(Reader& reader) :
		m_reader(reader)
	{
		m_items.reserve(2000);
		m_enemies.reserve(200);
		m_bullets.reserve(2000);
		m_lasers.reserve(200);
	}

	void Data::update()
	{
		m_reader.readPlayer(m_player);
		m_reader.readItems(m_items);
		m_reader.readEnemies(m_enemies);
		m_reader.readBullets(m_bullets);
		m_reader.readLasers(m_lasers);
	}

	void Data::print()
	{
		float_t maxDx = std::numeric_limits<float_t>::lowest();
		float_t maxDy = std::numeric_limits<float_t>::lowest();
		for (const Bullet& bullet : m_bullets)
		{
			if (bullet.dx > maxDx)
				maxDx = bullet.dx;

			if (bullet.dy > maxDy)
				maxDy = bullet.dy;
		}
		std::cout << maxDx << " " << maxDy << std::endl;
	}

	// ���ҵ���
	ItemTarget Data::findItem()
	{
		ItemTarget target = {};

		if (m_items.empty())
			return target;

		// ʰȡ��ȴ��
		if (std::chrono::duration_cast<std::chrono::milliseconds>(
			std::chrono::steady_clock::now() - m_findItemTime).count() < 3000)
			return target;

		// �Ի�����1/4��
		if (m_player.y < Scene::SIZE.height / 4.0f)
		{
			// ������ȴ
			m_findItemTime = std::chrono::steady_clock::now();
			return target;
		}

		// �Ի�����1/2������������10�������˶���5��
		if (m_player.y < Scene::SIZE.height / 2.0f && m_items.size() < 10 && m_enemies.size() > 5)
		{
			// ������ȴ
			m_findItemTime = std::chrono::steady_clock::now();
			return target;
		}

		float_t minDist = std::numeric_limits<float_t>::max();
		//float_t maxY = std::numeric_limits<float_t>::lowest();
		for (const Item& item : m_items)
		{
			if (!Scene::IsInScene(item.getPosition()))
				continue;

			// ���߸���1/5��
			if (item.y < Scene::SIZE.height / 5.0f)
				continue;

			// ���߲����Ի�1/4����
			float_t dy = std::abs(item.y - m_player.y);
			if (dy > Scene::SIZE.height / 4.0f)
				continue;

			// ����̫�����л�
			bool tooClose = false;
			for (const Enemy& enemy : m_enemies)
			{
				if (item.calcDistance(enemy.getPosition()) < 100.0f)
				{
					tooClose = true;
					break;
				}
			}
			if (tooClose)
				continue;

			// �������Ի��������
			float_t dist = item.calcDistance(m_player.getPosition());
			if (dist < minDist)
			{
				minDist = dist;
				target.found = true;
				target.item = item;
			}

			//if (item.y > maxY)
			//{
			//	maxY = item.y;
			//	target.found = true;
			//	target.item = item;
			//}
		}

		return target;
	}

	bool Data::hasEnemy() const
	{
		return !m_enemies.empty();
	}

	bool Data::isBoss() const
	{
		return m_enemies.size() == 1 && m_enemies[0].isBoss();
	}

	bool Data::isTalking() const
	{
		return (m_enemies.empty() || isBoss()) && m_bullets.empty() && m_lasers.empty();
	}

	bool Data::isUnderEnemy() const
	{
		bool underEnemy = false;

		for (const Enemy& enemy : m_enemies)
		{
			if (std::abs(m_player.x - enemy.x) < 16.0f && m_player.y > enemy.y)
			{
				underEnemy = true;
				break;
			}
		}

		return underEnemy;
	}

	// ���ҵ���
	EnemyTarget Data::findEnemy()
	{
		EnemyTarget target = {};

		if (m_enemies.empty())
			return target;

		// �Ի�����1/4��
		if (m_player.y < Scene::SIZE.height / 4.0f)
			return target;

		float_t minDist = std::numeric_limits<float_t>::max();
		for (const Enemy& enemy : m_enemies)
		{
			if (!Scene::IsInScene(enemy.getPosition()))
				continue;

			if (enemy.y > m_player.y)
				continue;

			// ���Ի�X��������
			float_t dx = std::abs(enemy.x - m_player.x);
			if (dx < minDist)
			{
				minDist = dx;
				target.found = true;
				target.enemy = enemy;
			}
		}

		return target;
	}

	const Player& Data::getPlayer() const
	{
		return m_player;
	}

	const std::vector<Item>& Data::getItems() const
	{
		return m_items;
	}

	const std::vector<Enemy>& Data::getEnemies() const
	{
		return m_enemies;
	}

	const std::vector<Bullet>& Data::getBullets() const
	{
		return m_bullets;
	}

	const std::vector<Laser>& Data::getLasers() const
	{
		return m_lasers;
	}
}
