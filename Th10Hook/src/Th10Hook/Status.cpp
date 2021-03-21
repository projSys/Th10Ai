#include "Th10Hook/Status.h"

#include "Th10Hook/Th10Hook.h"

namespace th
{
	Status::Status() :
		statusFrame(0),
		inputFrame(0)
	{
		m_items.reserve(2000);
		m_enemies.reserve(200);
		m_bullets.reserve(2000);
		m_lasers.reserve(200);
	}

	void Status::update()
	{
		Th10Hook::ReadPlayer(m_player);
		Th10Hook::ReadItems(m_items);
		Th10Hook::ReadEnemies(m_enemies);
		Th10Hook::ReadBullets(m_bullets);
		Th10Hook::ReadLasers(m_lasers);
	}

	void Status::copy(const Status& other)
	{
		m_player = other.m_player;
		m_items.clear();
		for (const Item& item : other.m_items)
			m_items.push_back(item);
		m_enemies.clear();
		for (const Enemy& enemy : other.m_enemies)
			m_enemies.push_back(enemy);
		m_bullets.clear();
		for (const Bullet& bullet : other.m_bullets)
			m_bullets.push_back(bullet);
		m_lasers.clear();
		for (const Laser& laser : other.m_lasers)
			m_lasers.push_back(laser);

		statusFrame = other.statusFrame;
		inputFrame = other.inputFrame;
	}

	bool Status::hasEnemy() const
	{
		return !m_enemies.empty();
	}

	bool Status::isBoss() const
	{
		return m_enemies.size() == 1 && m_enemies[0].isBoss();
	}

	bool Status::isTalking() const
	{
		return (m_enemies.empty() || isBoss()) && m_bullets.empty() && m_lasers.empty();
	}

	bool Status::isUnderEnemy() const
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

	int_t Status::collide(const Player& player, float_t frame) const
	{
		for (const Bullet& org : m_bullets)
		{
			Bullet bullet = org;
			bullet.advance(frame);
			if (bullet.collide(player))
			{
				std::cout << statusFrame << "/" << inputFrame << "帧" << " 总数：" << m_bullets.size() << " 碰撞："
					<< "org(" << org.id << " " << org.x << " " << org.y << " " << org.dx << " " << org.dy << ") "
					<< "now(" << bullet.id << " " << bullet.x << " " << bullet.y << " " << bullet.dx << " " << bullet.dy << ") " << std::endl;
				return bullet.id;
			}
		}
		std::cout << statusFrame << "/" << inputFrame << "帧 不碰撞" << " 总数：" << m_bullets.size() << std::endl;
		return -1;
	}

	int_t Status::collide(const Player& player, float_t frame, int_t id) const
	{
		for (const Bullet& org : m_bullets)
		{
			if (org.id == id)
			{
				Bullet bullet = org;
				bullet.advance(frame);
				if (bullet.collide(player))
				{
					std::cout << statusFrame << "/" << inputFrame << "帧" << " 总数：" << m_bullets.size() << " 碰撞："
						<< "org(" << org.id << " " << org.x << " " << org.y << " " << org.dx << " " << org.dy << ") "
						<< "now(" << bullet.id << " " << bullet.x << " " << bullet.y << " " << bullet.dx << " " << bullet.dy << ") " << std::endl;
					return bullet.id;
				}
			}
		}
		std::cout << statusFrame << "/" << inputFrame << "帧" << " 找不到子弹：" << id << " 总数：" << m_bullets.size() << std::endl;
		return -1;
	}

	const Player& Status::getPlayer() const
	{
		return m_player;
	}

	const std::vector<Item>& Status::getItems() const
	{
		return m_items;
	}

	const std::vector<Enemy>& Status::getEnemies() const
	{
		return m_enemies;
	}

	const std::vector<Bullet>& Status::getBullets() const
	{
		return m_bullets;
	}

	const std::vector<Laser>& Status::getLasers() const
	{
		return m_lasers;
	}
}
