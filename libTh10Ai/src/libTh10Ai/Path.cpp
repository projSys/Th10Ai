#include "libTh10Ai/Common.h"
#include "libTh10Ai/Path.h"

namespace th
{
	Path::Path(Data& data, Scene& scene,
		ItemTarget& itemTarget, EnemyTarget& enemyTarget, bool underEnemy) :
		m_data(data),
		m_scene(scene),
		m_itemTarget(itemTarget),
		m_enemyTarget(enemyTarget),
		m_underEnemy(underEnemy),
		m_dir(DIR_NONE),
		m_bestScore(std::numeric_limits<float_t>::lowest()),
		m_bestDir(DIR_NONE),
		m_bestSlow(false),
		m_count(0)
	{
	}

	Result Path::find(Direction dir)
	{
		m_dir = dir;

		Action action;
		action.fromPos = m_data.getPlayer().getPosition();
		action.fromDir = m_dir;
		action.slowFirst = (!m_itemTarget.found && m_underEnemy);
		action.frame = 1.0f;

		action.willCollideCount = 0;
		action.minCollideFrame = 0.0f;

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
		m_count += 1;
		if (m_count > FIND_LIMIT)
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
			{
				return result;
			}
		}

		result.valid = true;

		//result.score = action.minCollideFrame;
		//if (collideResult.willCollideCount > 0)
		//	result.score += collideResult.minCollideFrame;
		//result.score /= action.frame;

		if (m_itemTarget.found)
		{
			result.score = calcCollectScore(temp);
		}
		else if (m_enemyTarget.found)
		{
			result.score = calcShootScore(temp);
		}
		else
		{
			result.score = calcGobackScore(temp);
		}

		if (result.score > m_bestScore)
		{
			m_bestScore = result.score;
		}

		result.size = FIND_SIZES[m_dir];
		for (int_t i = 0; i < FIND_SIZES[m_dir]; ++i)
		{
			Direction dir = FIND_DIRS[m_dir][i];

			Action nextAct;
			nextAct.fromPos = temp.getPosition();
			nextAct.fromDir = dir;
			nextAct.slowFirst = action.slowFirst;
			nextAct.frame = action.frame + 1.0f;

			//nextAct.willCollideCount = action.willCollideCount;
			//nextAct.minCollideFrame = action.minCollideFrame;
			//if (collideResult.willCollideCount > 0)
			//{
			//	nextAct.willCollideCount += collideResult.willCollideCount;
			//	nextAct.minCollideFrame += collideResult.minCollideFrame;
			//}

			Result nextRes = dfs(nextAct);

			if (m_count > FIND_LIMIT)
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
			if (dx > Scene::SIZE.width)
				dx = Scene::SIZE.width;
			float_t dy = std::abs(player.y - item.y);
			if (dy > Scene::SIZE.height)
				dy = Scene::SIZE.height;

			score += 150.0f * (1.0f - dx / Scene::SIZE.width);
			score += 150.0f * (1.0f - dy / Scene::SIZE.height);
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
		if (dx > Scene::SIZE.width)
			dx = Scene::SIZE.width;
		if (dx < 16.0f)
		{
			score += 100.0f;
		}
		else
		{
			// X轴距离越近得分越高
			score += 100.0f * (1.0f - dx / Scene::SIZE.width);
		}

		float_t dy = std::abs(player.y - enemy.y);
		if (dy > Scene::SIZE.height)
			dy = Scene::SIZE.height;
		if (dy > Scene::SIZE.height / 2.0f)
		{
			score += 100.0f;
		}
		else
		{
			// Y轴距离越远得分越高
			score += 100.0f * (dy / Scene::SIZE.height);
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
			if (dx > Scene::SIZE.width)
				dx = Scene::SIZE.width;
			float_t dy = std::abs(player.y - Player::INIT_POS.y);
			if (dy > Scene::SIZE.height)
				dy = Scene::SIZE.height;

			score += 50.0f * (1.0f - dx / Scene::SIZE.width);
			score += 50.0f * (1.0f - dy / Scene::SIZE.height);
		}

		return score;
	}
}
