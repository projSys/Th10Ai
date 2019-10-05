#include "libTH10AI/Common.h"
#include "libTH10AI/Path.h"

#include "libTH10AI/Mover.h"

namespace th
{
	Path::Path(Data& data, Scene& scene, Direction pathDir,
		const ItemTarget& itemTarget, const EnemyTarget& enemyTarget) :
		m_data(data),
		m_scene(scene),
		m_pathDir(pathDir),
		m_itemTarget(itemTarget),
		m_enemyTarget(enemyTarget),
		m_bestScore(std::numeric_limits<float_t>::lowest()),
		m_bestDir(DIR_NONE),
		m_bestSlow(false),
		m_count(0),
		m_limit(500)
	{
	}

	Result Path::find(bool underEnemy)
	{
		Action action;
		action.fromPos = m_data.getPlayer().getPosition();
		action.fromDir = m_pathDir;
		action.slowFirst = (!m_itemTarget.found && underEnemy);
		action.frame = 1.0f;

		return dfs(action);
	}

	Result Path::dfs(const Action& action)
	{
		Result result;
		result.valid = false;
		result.slow = false;
		result.score = 0.0f;
		result.size = 0;

		// 超过搜索节点限制
		++m_count;
		if (m_count >= m_limit)
			return result;

		CellCollideResult collideResult = {};
		// 前进到下一个坐标
		Player temp = m_data.getPlayer();
		temp.setPosition(action.fromPos);
		temp.advance(action.fromDir, action.slowFirst);
		result.slow = action.slowFirst;
		if (!Scene::IsInPlayerArea(temp.getPosition()) || (collideResult = m_scene.collideAll(temp, action.frame)).collided)
		{
			temp.setPosition(action.fromPos);
			temp.advance(action.fromDir, !action.slowFirst);
			result.slow = !action.slowFirst;
			if (!Scene::IsInPlayerArea(temp.getPosition()) || (collideResult = m_scene.collideAll(temp, action.frame)).collided)
				return result;
		}

		if (m_pathDir == DIR_HOLD && collideResult.willCollideCount > m_data.getBullets().size() * 0.1)
		{
			m_itemTarget.found = false;
			m_enemyTarget.found = false;
			std::cout << "被瞄准了，快跑。" << std::endl;
			return result;
		}

		result.valid = true;

		if (m_itemTarget.found)
		{
			result.score += calcCollectScore(temp);
		}
		else if (m_enemyTarget.found)
		{
			result.score += calcShootScore(temp);
		}
		else
		{
			result.score += calcGobackScore(temp);
		}

		if (result.score > m_bestScore)
		{
			m_bestScore = result.score;
		}

		Mover mover(m_pathDir);
		result.size = mover.getSize();
		while (mover.hasNext())
		{
			Direction dir = mover.next();

			Action nextAct;
			nextAct.fromPos = temp.getPosition();
			nextAct.fromDir = dir;
			nextAct.slowFirst = action.slowFirst;
			nextAct.frame = action.frame + 1.0f;

			Result nextRes = dfs(nextAct);

			if (m_count >= m_limit)
			{
				break;
			}

			if (!nextRes.valid)
			{
				result.size -= 1;
				continue;
			}
		}
		// 没气了，当前节点也无效
		if (result.size <= 0)
			result.valid = false;

		return result;
	}

	// 拾取道具评分
	float_t Path::calcCollectScore(const Player& player)
	{
		float_t score = 0.0f;

		if (!m_itemTarget.found)
			return score;

		const Item& item = m_itemTarget.item;

		if (player.calcDistance(item.getPosition()) < 8.0f)
		{
			score += 300.0f;
		}
		else
		{
			float_t dx = std::abs(player.x - item.x);
			if (dx > SCENE_SIZE.width)
				dx = SCENE_SIZE.width;
			float_t dy = std::abs(player.y - item.y);
			if (dy > SCENE_SIZE.height)
				dy = SCENE_SIZE.height;

			score += 150.0f * (1.0f - dx / SCENE_SIZE.width);
			score += 150.0f * (1.0f - dy / SCENE_SIZE.height);
		}

		return score;
	}

	// 攻击敌人评分
	float_t Path::calcShootScore(const Player& player)
	{
		float_t score = 0.0f;

		if (!m_enemyTarget.found)
			return score;

		const Enemy& enemy = m_enemyTarget.enemy;

		float_t dx = std::abs(player.x - enemy.x);
		if (dx > SCENE_SIZE.width)
			dx = SCENE_SIZE.width;
		if (dx < 16.0f)
		{
			score += 100.0f;
		}
		else
		{
			// X轴距离越近得分越高
			score += 100.0f * (1.0f - dx / SCENE_SIZE.width);
		}

		float_t dy = std::abs(player.y - enemy.y);
		if (dy > SCENE_SIZE.height)
			dy = SCENE_SIZE.height;
		if (dy > SCENE_SIZE.height / 2.0f)
		{
			score += 100.0f;
		}
		else
		{
			// Y轴距离越远得分越高
			score += 100.0f * (dy / SCENE_SIZE.height);
		}

		return score;
	}

	float_t Path::calcGobackScore(const Player& player)
	{
		float_t score = 0.0f;

		if (player.calcDistance(Player::INIT_POS) < 8.0f)
		{
			score += 100.0f;
		}
		else
		{
			float_t dx = std::abs(player.x - Player::INIT_POS.x);
			if (dx > SCENE_SIZE.width)
				dx = SCENE_SIZE.width;
			float_t dy = std::abs(player.y - Player::INIT_POS.y);
			if (dy > SCENE_SIZE.height)
				dy = SCENE_SIZE.height;

			score += 50.0f * (1.0f - dx / SCENE_SIZE.width);
			score += 50.0f * (1.0f - dy / SCENE_SIZE.height);
		}

		return score;
	}
}
