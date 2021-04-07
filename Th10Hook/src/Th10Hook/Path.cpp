#include "Th10Hook/Path.h"

namespace th
{
	const DIR Path::FIND_DIRS[enum_cast(DIR::MAX_COUNT)][5] =
	{
		// DIR::HOLD
		{ DIR::HOLD,      DIR::UP,       DIR::DOWN,      DIR::LEFT,      DIR::RIGHT,     },//DIR::LEFTUP,  DIR::RIGHTUP,  DIR::LEFTDOWN,  DIR::RIGHTDOWN },
		// DIR::UP
		{ DIR::UP,        DIR::LEFTUP,   DIR::RIGHTUP,   DIR::LEFT,      DIR::RIGHT,     },//DIR::HOLD,    DIR::HOLD,     DIR::HOLD,      DIR::HOLD      },
		// DIR::DOWN
		{ DIR::DOWN,      DIR::LEFTDOWN, DIR::RIGHTDOWN, DIR::LEFT,      DIR::RIGHT,     },//DIR::HOLD,    DIR::HOLD,     DIR::HOLD,      DIR::HOLD      },
		// DIR::LEFT
		{ DIR::LEFT,      DIR::LEFTUP,   DIR::LEFTDOWN,  DIR::UP,        DIR::DOWN,      },//DIR::HOLD,    DIR::HOLD,     DIR::HOLD,      DIR::HOLD      },
		// DIR::RIGHT
		{ DIR::RIGHT,     DIR::RIGHTUP,  DIR::RIGHTDOWN, DIR::UP,        DIR::DOWN,      },//DIR::HOLD,    DIR::HOLD,     DIR::HOLD,      DIR::HOLD      },
		// DIR::LEFTUP
		{ DIR::LEFTUP,    DIR::LEFT,     DIR::UP,        DIR::LEFTDOWN,  DIR::RIGHTUP,   },//DIR::HOLD,    DIR::HOLD,     DIR::HOLD,      DIR::HOLD      },
		// DIR::RIGHTUP
		{ DIR::RIGHTUP,   DIR::UP,       DIR::RIGHT,     DIR::LEFTUP,    DIR::RIGHTDOWN, },//DIR::HOLD,    DIR::HOLD,     DIR::HOLD,      DIR::HOLD      },
		// DIR::LEFTDOWN
		{ DIR::LEFTDOWN,  DIR::DOWN,     DIR::LEFT,      DIR::RIGHTDOWN, DIR::LEFTUP,    },//DIR::HOLD,    DIR::HOLD,     DIR::HOLD,      DIR::HOLD      },
		// DIR::RIGHTDOWN
		{ DIR::RIGHTDOWN, DIR::RIGHT,    DIR::DOWN,      DIR::RIGHTUP,   DIR::LEFTDOWN,  },//DIR::HOLD,    DIR::HOLD,     DIR::HOLD,      DIR::HOLD      }
	};

	const int_t Path::FIND_DIR_COUNTS[enum_cast(DIR::MAX_COUNT)] = { 1, 5, 5, 5, 5, 5, 5, 5, 5 };

	const int_t Path::FIND_LIMIT = 50;
	const float_t Path::FIND_DEPTH = 30;
	const vec2 Path::RESET_POS = { 0, 432 };

	Path::Path(Status& status, Scene& scene,
		const boost::optional<Item>& itemTarget,
		const boost::optional<Enemy>& enemyTarget,
		bool underEnemy) :
		m_status(status),
		m_scene(scene),
		m_itemTarget(itemTarget),
		m_enemyTarget(enemyTarget),
		m_underEnemy(underEnemy),
		m_dir(DIR::HOLD),
		m_slowFirst(false),
		m_bestScore(std::numeric_limits<float_t>::lowest()),
		m_count(0)
	{
	}

	Result Path::find(DIR dir)
	{
		m_dir = dir;
		//m_slowFirst = (!m_itemTarget.has_value() && m_underEnemy);

		Action action = {};
		action.fromPos = m_status.getPlayer().pos;
		action.fromDir = m_dir;
		action.frame = 1;

		//action.willCollideCount = 0;
		//action.minCollideFrame = 0;
		action.score = 0;

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
		Player player = m_status.getPlayer();
		player.pos = action.fromPos;
		player.advance(action.fromDir, m_slowFirst);
		result.slow = m_slowFirst;
		RegionCollideResult rcr = {};
		if (!Scene::IsInPlayerRegion(player.pos)
			|| (rcr = m_scene.collideAll(player, action.frame)).collided)
		{
			player.pos = action.fromPos;
			player.advance(action.fromDir, !m_slowFirst);
			result.slow = !m_slowFirst;
			if (!Scene::IsInPlayerRegion(player.pos)
				|| (rcr = m_scene.collideAll(player, action.frame)).collided)
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

		if (m_itemTarget.has_value())
		{
			result.score += CalcNearScore(player.pos, m_itemTarget.value().pos) * 100;
		}
		else if (m_enemyTarget.has_value())
		{
			result.score += CalcShootScore(player.pos, m_enemyTarget.value().pos) * 100;
		}
		else
		{
			result.score += CalcNearScore(player.pos, RESET_POS) * 100;
		}

		//if (ccr.minDistance > 8)
		//	result.score += 150;
		//else
		//	result.score += ccr.minDistance / 8 * 150;

		float_t total = action.score + result.score;
		float_t avg = total / action.frame;
		if (avg > m_bestScore)
			m_bestScore = avg;

		int_t nextValidCount = FIND_DIR_COUNTS[enum_cast(m_dir)];
		for (int_t i = 0; i < FIND_DIR_COUNTS[enum_cast(m_dir)]; ++i)
		{
			DIR dir = FIND_DIRS[enum_cast(m_dir)][i];

			Action nextAct = {};
			nextAct.fromPos = player.pos;
			nextAct.fromDir = dir;
			nextAct.frame = action.frame + 1;
			nextAct.score = total;

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

	float_t Path::CalcFarScore(vec2 player, vec2 target)
	{
		float_t score = 0;

		// 坐标原点移到左上角
		player += Scene::ORIGIN_POINT_OFFSET;
		target += Scene::ORIGIN_POINT_OFFSET;

		// 距离越远得分越高
		if (player.x < target.x)
			score += 0.5f * ((target.x - player.x) / target.x);
		else
			score += 0.5f * ((player.x - target.x) / (Scene::SIZE.x - target.x));

		if (player.y < target.y)
			score += 0.5f * ((target.y - player.y) / target.y);
		else
			score += 0.5f * ((player.y - target.y) / (Scene::SIZE.y - target.y));

		return score;
	}

	float_t Path::CalcNearScore(vec2 player, vec2 target)
	{
		float_t score = 0;

		// 坐标原点移到左上角
		player += Scene::ORIGIN_POINT_OFFSET;
		target += Scene::ORIGIN_POINT_OFFSET;

		// 距离越近得分越高
		if (player.x < target.x)
			score += 0.5f * (1 - (target.x - player.x) / target.x);
		else
			score += 0.5f * (1 - (player.x - target.x) / (Scene::SIZE.x - target.x));

		if (player.y < target.y)
			score += 0.5f * (1 - (target.y - player.y) / target.y);
		else
			score += 0.5f * (1 - (player.y - target.y) / (Scene::SIZE.y - target.y));

		return score;
	}

	float_t Path::CalcShootScore(vec2 player, vec2 target)
	{
		float_t score = 0;

		// 坐标原点移到左上角
		player += Scene::ORIGIN_POINT_OFFSET;
		target += Scene::ORIGIN_POINT_OFFSET;

		// 距离越近得分越高
		if (player.x < target.x)
			score += 0.5f * (1 - (target.x - player.x) / target.x);
		else
			score += 0.5f * (1 - (player.x - target.x) / (Scene::SIZE.x - target.x));

		// 距离越远得分越高
		if (player.y < target.y)
			score = -1;
		else
			score += 0.5f * ((player.y - target.y) / (Scene::SIZE.y - target.y));

		return score;
	}
}
