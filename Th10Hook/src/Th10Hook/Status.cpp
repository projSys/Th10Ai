#include "Th10Hook/Status.h"

#include "Th10Hook/Th10Hook.h"

namespace th
{
	Status::Status() :
		inputFrame(0),
		statusFrame(0),
		handleFrame(0)
	{
		m_items.reserve(2000);
		m_enemies.reserve(200);
		m_bullets.reserve(2000);
		m_lasers.reserve(200);
	}

	void Status::clear()
	{
		m_player = {};
		m_items.clear();
		m_enemies.clear();
		m_bullets.clear();
		m_lasers.clear();
	}

	void Status::update()
	{
		Th10Hook::GetPlayer(m_player);
		Th10Hook::GetItems(m_items);
		Th10Hook::GetEnemies(m_enemies);
		Th10Hook::GetBullets(m_bullets);
		Th10Hook::GetLasers(m_lasers);

		frame1 = m_player.stageFrame;
	}

	void Status::updateExtra()
	{
		m_player.updateExtra();
		for (Item& item : m_items)
			item.updateExtra();
		for (Enemy& enemy : m_enemies)
			enemy.updateExtra();
		for (Bullet& bullet : m_bullets)
			bullet.updateExtra();
		for (Laser& laser : m_lasers)
			laser.updateExtra();
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

		inputFrame = other.inputFrame;
		statusFrame = other.statusFrame;
		handleFrame = other.handleFrame;
		frame1 = other.frame1;
	}

	bool Status::haveEnemies() const
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
			if (std::abs(m_player.pos.x - enemy.pos.x) < 16 && m_player.pos.y > enemy.pos.y)
			{
				underEnemy = true;
				break;
			}
		}
		return underEnemy;
	}

	boost::optional<Bullet> Status::collide(const Player& player, float_t frame) const
	{
		boost::optional<Bullet> target;
		for (const Bullet& org : m_bullets)
		{
			Bullet now = org;
			now.advance(frame);
			if (now.collide(player))
			{
				std::cout << statusFrame - handleFrame << "/" << handleFrame << "/" << inputFrame - handleFrame << "/" << frame1 - handleFrame << "帧"
					<< " 总数：" << m_bullets.size() << " 碰撞："
					<< "org(" << org.id << " " << org.pos.x << " " << org.pos.y << " " << org.delta.x << " " << org.delta.y << ") "
					<< "now(" << now.id << " " << now.pos.x << " " << now.pos.y << " " << now.delta.x << " " << now.delta.y << ") " << std::endl;
				target = now;
				return target;
			}
		}
		std::cout << statusFrame - handleFrame << "/" << handleFrame << "/" << inputFrame - handleFrame << "/" << frame1 - handleFrame << "帧"
			<< " 总数：" << m_bullets.size() << " 不碰撞" << std::endl;
		return target;
	}

	int_t Status::collide(const Player& player, float_t frame, int_t id) const
	{
		for (const Bullet& org : m_bullets)
		{
			if (org.id == id)
			{
				Bullet now = org;
				now.advance(frame);
				if (now.collide(player))
				{
					std::cout << statusFrame - handleFrame << "/" << handleFrame << "/" << inputFrame - handleFrame << "/" << frame1 - handleFrame << "帧"
						<< " 总数：" << m_bullets.size() << " 碰撞："
						<< "org(" << org.id << " " << org.pos.x << " " << org.pos.y << " " << org.delta.x << " " << org.delta.y << ") "
						<< "now(" << now.id << " " << now.pos.x << " " << now.pos.y << " " << now.delta.x << " " << now.delta.y << ") " << std::endl;
					return now.id;
				}
			}
		}
		std::cout << statusFrame - handleFrame << "/" << handleFrame << "/" << inputFrame - handleFrame << "/" << frame1 - handleFrame << "帧"
			<< " 总数：" << m_bullets.size() << " 找不到子弹：" << id << std::endl;
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
