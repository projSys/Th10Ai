#include "libTh10Ai/Common.h"
#include "libTh10Ai/Path.h"

namespace th
{
	const Direction Path::FIND_DIRS[DIR_MAXCOUNT][DIR_MAXCOUNT] =
	{
		// DIR_HOLD
		{ DIR_HOLD,      DIR_UP,       DIR_DOWN,      DIR_LEFT,    DIR_RIGHT,     DIR_LEFTUP,  DIR_RIGHTUP,  DIR_LEFTDOWN,  DIR_RIGHTDOWN },
		// DIR_UP
		{ DIR_UP,        DIR_LEFTUP,   DIR_RIGHTUP,   DIR_LEFT,    DIR_RIGHT,     DIR_NONE,    DIR_NONE,     DIR_NONE,      DIR_NONE      },
		// DIR_DOWN
		{ DIR_DOWN,      DIR_LEFTDOWN, DIR_RIGHTDOWN, DIR_LEFT,    DIR_RIGHT,     DIR_NONE,    DIR_NONE,     DIR_NONE,      DIR_NONE      },
		// DIR_LEFT
		{ DIR_LEFT,      DIR_LEFTUP,   DIR_LEFTDOWN,  DIR_UP,      DIR_DOWN,      DIR_NONE,    DIR_NONE,     DIR_NONE,      DIR_NONE      },
		// DIR_RIGHT
		{ DIR_RIGHT,     DIR_RIGHTUP,  DIR_RIGHTDOWN, DIR_UP,      DIR_DOWN,      DIR_NONE,    DIR_NONE,     DIR_NONE,      DIR_NONE      },
		// DIR_UPLEFT
		{ DIR_LEFTUP,    DIR_UP,       DIR_LEFT,      DIR_RIGHTUP, DIR_LEFTDOWN,  DIR_NONE,    DIR_NONE,     DIR_NONE,      DIR_NONE      },
		// DIR_UPRIGHT
		{ DIR_RIGHTUP,   DIR_UP,       DIR_RIGHT,     DIR_LEFTUP,  DIR_RIGHTDOWN, DIR_NONE,    DIR_NONE,     DIR_NONE,      DIR_NONE      },
		// DIR_DOWNLEFT
		{ DIR_LEFTDOWN,  DIR_DOWN,     DIR_LEFT,      DIR_LEFTUP,  DIR_RIGHTDOWN, DIR_NONE,    DIR_NONE,     DIR_NONE,      DIR_NONE      },
		// DIR_DOWNRIGHT
		{ DIR_RIGHTDOWN, DIR_DOWN,     DIR_RIGHT,     DIR_RIGHTUP, DIR_LEFTDOWN,  DIR_NONE,    DIR_NONE,     DIR_NONE,      DIR_NONE      }
	};

	const int_t Path::FIND_SIZES[DIR_MAXCOUNT] = { 1, 5, 5, 5, 5, 5, 5, 5, 5 };

	const int_t Path::FIND_LIMIT = 500;
	const float_t Path::FIND_DEPTH = 30.0f;
	const Pointf Path::RESET_POS = { 0.0f, 432.0f };

	Path::Path(Data& data, Scene& scene,
		ItemTarget& itemTarget, EnemyTarget& enemyTarget, bool underEnemy) :
		m_data(data),
		m_scene(scene),
		m_itemTarget(itemTarget),
		m_enemyTarget(enemyTarget),
		m_underEnemy(underEnemy),
		m_dir(DIR_NONE),
		m_slowFirst(false),
		m_bestScore(std::numeric_limits<float_t>::lowest()),
		m_count(0)
	{
	}

	Result Path::find(Direction dir)
	{
		m_dir = dir;
		//m_slowFirst = (!m_itemTarget.found && m_underEnemy);
		m_slowFirst = false;

		Action action = {};
		action.fromPos = m_data.getPlayer().getPosition();
		action.fromDir = m_dir;
		action.frame = 1.0f;

		//action.willCollideCount = 0;
		//action.minCollideFrame = 0.0f;

		return dfs(action);
	}

	Result Path::dfs(const Action& action)
	{
		Result result = {};

		// 超过搜索节点限制
		++m_count;
		if (m_count > FIND_LIMIT)
			return result;

		if (action.frame > FIND_DEPTH)
			return result;

		// 前进到下一个坐标
		Player temp = m_data.getPlayer();
		temp.setPosition(action.fromPos);
		temp.advance(action.fromDir, m_slowFirst);
		result.slow = m_slowFirst;
		CellCollideResult ccr = {};
		if (!Scene::IsInPlayerArea(temp.getPosition()) || (ccr = m_scene.collideAll(temp, action.frame)).collided)
		{
			temp.setPosition(action.fromPos);
			temp.advance(action.fromDir, !m_slowFirst);
			result.slow = !m_slowFirst;
			if (!Scene::IsInPlayerArea(temp.getPosition()) || (ccr = m_scene.collideAll(temp, action.frame)).collided)
			{
				//result.ttd = 10;
				return result;
			}
		}

		result.valid = true;

		//result.score = action.minCollideFrame;
		//if (ccResult.willCollideCount > 0)
		//	result.score += ccResult.minCollideFrame;
		//result.score /= action.frame;

		if (m_itemTarget.found)
		{
			result.score += CalcNearScore(temp.getPosition(), m_itemTarget.item.getPosition()) * 300.0f;
		}
		else if (m_enemyTarget.found)
		{
			result.score += CalcShootScore(temp.getPosition(), m_enemyTarget.enemy.getPosition()) * 200.0f;
		}

		result.score += CalcNearScore(temp.getPosition(), RESET_POS) * 50.0f;
		//result.score += CalcDepthScore(action.frame) * 100.0f;

		if (result.score > m_bestScore)
		{
			m_bestScore = result.score;
		}

		int_t nextValidCount = FIND_SIZES[m_dir];
		for (int_t i = 0; i < FIND_SIZES[m_dir]; ++i)
		{
			Direction dir = FIND_DIRS[m_dir][i];

			Action nextAct = {};
			nextAct.fromPos = temp.getPosition();
			nextAct.fromDir = dir;
			nextAct.frame = action.frame + 1.0f;

			//nextAct.willCollideCount = action.willCollideCount;
			//nextAct.minCollideFrame = action.minCollideFrame;
			//if (ccResult.willCollideCount > 0)
			//{
			//	nextAct.willCollideCount += ccResult.willCollideCount;
			//	nextAct.minCollideFrame += ccResult.minCollideFrame;
			//}

			Result nextRes = dfs(nextAct);

			if (m_count > FIND_LIMIT)
				break;

			//if (!nextRes.valid)
			//{
			//	int_t ttd = nextRes.ttd - 1;
			//	if (ttd > result.ttd)
			//		result.ttd = ttd;
			//}
			if (!nextRes.valid)
				nextValidCount -= 1;
		}
		// 没气了，当前节点也无效
		//if (result.ttd > 0)
		//	result.valid = false;
		if (nextValidCount == 0)
			result.valid = false;

		return result;
	}

	float_t Path::CalcFarScore(Pointf player, Pointf target)
	{
		float_t score = 0.0f;

		// 坐标原点移到左上角
		player.x += (Scene::SIZE.width / 2.0f);
		target.x += (Scene::SIZE.width / 2.0f);

		// 距离越远得分越高
		if (player.x < target.x)
			score += 0.5f * ((target.x - player.x) / target.x);
		else
			score += 0.5f * ((player.x - target.x) / (Scene::SIZE.width - target.x));

		if (player.y < target.y)
			score += 0.5f * ((target.y - player.y) / target.y);
		else
			score += 0.5f * ((player.y - target.y) / (Scene::SIZE.height - target.y));

		return score;
	}

	float_t Path::CalcNearScore(Pointf player, Pointf target)
	{
		float_t score = 0.0f;

		// 坐标原点移到左上角
		player.x += (Scene::SIZE.width / 2.0f);
		target.x += (Scene::SIZE.width / 2.0f);

		// 距离越近得分越高
		if (player.x < target.x)
			score += 0.5f * (1.0f - (target.x - player.x) / target.x);
		else
			score += 0.5f * (1.0f - (player.x - target.x) / (Scene::SIZE.width - target.x));

		if (player.y < target.y)
			score += 0.5f * (1.0f - (target.y - player.y) / target.y);
		else
			score += 0.5f * (1.0f - (player.y - target.y) / (Scene::SIZE.height - target.y));

		return score;
	}

	float_t Path::CalcShootScore(Pointf player, Pointf enemy)
	{
		float_t score = 0.0f;

		// 坐标原点移到左上角
		player.x += (Scene::SIZE.width / 2.0f);
		enemy.x += (Scene::SIZE.width / 2.0f);

		// 距离越近得分越高
		if (player.x < enemy.x)
			score += 0.5f * (1.0f - (enemy.x - player.x) / enemy.x);
		else
			score += 0.5f * (1.0f - (player.x - enemy.x) / (Scene::SIZE.width - enemy.x));

		// 距离越远得分越高
		if (player.y <= enemy.y)
			score = -1.0f;
		else
			score += 0.5f * ((player.y - enemy.y) / (Scene::SIZE.height - enemy.y));

		return score;
	}

	float_t Path::CalcDepthScore(float_t frame)
	{
		float_t score = 0.0f;

		score = frame / FIND_DEPTH;

		return score;
	}
}
