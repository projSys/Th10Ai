#include "Th10Ai/Path.h"

namespace th
{
	const Direction Path::FIND_DIRS[to_underlying(Direction::MAX_COUNT)][5] =
	{
		// Direction::HOLD
		{ Direction::HOLD,       Direction::UP,        Direction::DOWN,       Direction::LEFT,       Direction::RIGHT,      },//Direction::UP_LEFT, Direction::UP_RIGHT, Direction::DOWN_LEFT, Direction::DOWN_RIGHT },
		// Direction::UP
		{ Direction::UP,         Direction::UP_LEFT,   Direction::UP_RIGHT,   Direction::LEFT,       Direction::RIGHT,      },//Direction::HOLD,    Direction::HOLD,     Direction::HOLD,      Direction::HOLD      },
		// Direction::DOWN
		{ Direction::DOWN,       Direction::DOWN_LEFT, Direction::DOWN_RIGHT, Direction::LEFT,       Direction::RIGHT,      },//Direction::HOLD,    Direction::HOLD,     Direction::HOLD,      Direction::HOLD      },
		// Direction::LEFT
		{ Direction::LEFT,       Direction::UP_LEFT,   Direction::DOWN_LEFT,  Direction::UP,         Direction::DOWN,       },//Direction::HOLD,    Direction::HOLD,     Direction::HOLD,      Direction::HOLD      },
		// Direction::RIGHT
		{ Direction::RIGHT,      Direction::UP_RIGHT,  Direction::DOWN_RIGHT, Direction::UP,         Direction::DOWN,       },//Direction::HOLD,    Direction::HOLD,     Direction::HOLD,      Direction::HOLD      },
		// Direction::UP_LEFT
		{ Direction::UP_LEFT,    Direction::LEFT,      Direction::UP,         Direction::DOWN_LEFT,  Direction::UP_RIGHT,   },//Direction::HOLD,    Direction::HOLD,     Direction::HOLD,      Direction::HOLD      },
		// Direction::UP_RIGHT
		{ Direction::UP_RIGHT,   Direction::UP,        Direction::RIGHT,      Direction::UP_LEFT,    Direction::DOWN_RIGHT, },//Direction::HOLD,    Direction::HOLD,     Direction::HOLD,      Direction::HOLD      },
		// Direction::DOWN_LEFT
		{ Direction::DOWN_LEFT,  Direction::DOWN,      Direction::LEFT,       Direction::DOWN_RIGHT, Direction::UP_LEFT,    },//Direction::HOLD,    Direction::HOLD,     Direction::HOLD,      Direction::HOLD      },
		// Direction::DOWN_RIGHT
		{ Direction::DOWN_RIGHT, Direction::RIGHT,     Direction::DOWN,       Direction::UP_RIGHT,   Direction::DOWN_LEFT,  },//Direction::HOLD,    Direction::HOLD,     Direction::HOLD,      Direction::HOLD      }
	};

	const int_t Path::FIND_DIR_COUNTS[to_underlying(Direction::MAX_COUNT)] = { 1, 5, 5, 5, 5, 5, 5, 5, 5 };

	const int_t Path::FIND_LIMIT = 120;
	const float_t Path::FIND_DEPTH = 40;
	const vec2 Path::RESET_POS = vec2(_F(0.0), _F(431.0));

	Path::Path(Status& status, Scene& scene,
		const std::optional<Item>& itemTarget,
		const std::optional<Enemy>& enemyTarget,
		bool underEnemy) :
		m_status(status),
		m_scene(scene),
		m_itemTarget(itemTarget),
		m_enemyTarget(enemyTarget),
		m_underEnemy(underEnemy),
		m_dir(Direction::HOLD),
		m_slowFirst(false),
		m_bestScore(std::numeric_limits<float_t>::lowest()),
		m_count(0)
	{
	}

	Result Path::find(Direction dir)
	{
		m_dir = dir;
		m_slowFirst = (!m_itemTarget.has_value() && m_underEnemy);

		Action action = {};
		action.fromPos = m_status.getPlayer().pos;
		action.fromDir = m_dir;
		action.frame = 1;
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
		player.setPosition(action.fromPos);
		player.move(action.fromDir, m_slowFirst);
		result.slow = m_slowFirst;
		RegionCollideResult rcr = {};
		if (!Scene::IsInPlayerRegion(player.pos)
			|| (rcr = m_scene.collideAll(player, action.frame)).collided)
		{
			player.setPosition(action.fromPos);
			player.move(action.fromDir, !m_slowFirst);
			result.slow = !m_slowFirst;
			if (!Scene::IsInPlayerRegion(player.pos)
				|| (rcr = m_scene.collideAll(player, action.frame)).collided)
			{
				return result;
			}
		}

		result.valid = true;

		if (m_itemTarget.has_value())
		{
			result.score += CalcNearScore(player.pos, m_itemTarget.value().pos) * _F(100.0);
		}
		else if (m_enemyTarget.has_value())
		{
			result.score += CalcShootScore(player.pos, m_enemyTarget.value().pos) * _F(100.0);
		}
		else
		{
			result.score += CalcNearScore(player.pos, RESET_POS) * _F(100.0);
		}

		float_t total = action.score + result.score;
		float_t avg = total / action.frame;
		if (avg > m_bestScore)
		{
			m_bestScore = avg;
		}

		int_t nextValidCount = FIND_DIR_COUNTS[to_underlying(m_dir)];
		for (int_t i = 0; i < FIND_DIR_COUNTS[to_underlying(m_dir)]; ++i)
		{
			Direction dir = FIND_DIRS[to_underlying(m_dir)][i];

			Action nextAct = {};
			nextAct.fromPos = player.pos;
			nextAct.fromDir = dir;
			nextAct.frame = action.frame + 1;
			nextAct.score = total;

			Result nextRes = dfs(nextAct);

			if (m_count > FIND_LIMIT)
				break;

			if (!nextRes.valid)
				nextValidCount -= 1;
		}
		// 没气了，当前节点也无效
		if (nextValidCount == 0)
			result.valid = false;

		return result;
	}

	float_t Path::CalcFarScore(vec2 player, vec2 target)
	{
		float_t score = _F(0.0);

		// 坐标原点移到左上角
		player += Scene::ORIGIN_POINT_OFFSET;
		target += Scene::ORIGIN_POINT_OFFSET;

		// 距离越远得分越高
		if (player.x < target.x)
			score += _F(0.5) * ((target.x - player.x) / target.x);
		else
			score += _F(0.5) * ((player.x - target.x) / (Scene::SIZE.x - target.x));

		if (player.y < target.y)
			score += _F(0.5) * ((target.y - player.y) / target.y);
		else
			score += _F(0.5) * ((player.y - target.y) / (Scene::SIZE.y - target.y));

		return score;
	}

	float_t Path::CalcNearScore(vec2 player, vec2 target)
	{
		float_t score = _F(0.0);

		// 坐标原点移到左上角
		player += Scene::ORIGIN_POINT_OFFSET;
		target += Scene::ORIGIN_POINT_OFFSET;

		// 距离越近得分越高
		if (player.x < target.x)
			score += _F(0.5) * (1 - (target.x - player.x) / target.x);
		else
			score += _F(0.5) * (1 - (player.x - target.x) / (Scene::SIZE.x - target.x));

		if (player.y < target.y)
			score += _F(0.5) * (1 - (target.y - player.y) / target.y);
		else
			score += _F(0.5) * (1 - (player.y - target.y) / (Scene::SIZE.y - target.y));

		return score;
	}

	float_t Path::CalcShootScore(vec2 player, vec2 target)
	{
		float_t score = _F(0.0);

		// 坐标原点移到左上角
		player += Scene::ORIGIN_POINT_OFFSET;
		target += Scene::ORIGIN_POINT_OFFSET;

		// 距离越近得分越高
		if (player.x < target.x)
			score += _F(0.5) * (1 - (target.x - player.x) / target.x);
		else
			score += _F(0.5) * (1 - (player.x - target.x) / (Scene::SIZE.x - target.x));

		// 距离越远得分越高
		if (player.y < target.y)
			score += _F(-1.0);
		else
			score += _F(0.5) * ((player.y - target.y) / (Scene::SIZE.y - target.y));

		return score;
	}
}
