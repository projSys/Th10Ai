#pragma once

#include "Th10Ai/Common.h"

#include <boost/optional.hpp>

#include "Th10Ai/Status.h"
#include "Th10Ai/Scene.h"

namespace th
{
	struct Action
	{
		vec2 fromPos;
		DIR fromDir;
		int_t frame;

		//int_t willCollideCount;
		//float_t minCollideFrame;
		float_t score;
	};

	struct Result
	{
		bool valid;
		bool slow;		// 实际是否慢速
		float_t score;
		//int_t ttd;
	};

	class Path
	{
	public:
		Path(Status& status, Scene& scene,
			const boost::optional<Item>& itemTarget,
			const boost::optional<Enemy>& enemyTarget,
			bool underEnemy);

		Result find(DIR dir);
		Result dfs(const Action& action);

	//private:
		static float_t CalcFarScore(vec2 player, vec2 target);
		static float_t CalcNearScore(vec2 player, vec2 target);
		static float_t CalcShootScore(vec2 player, vec2 target);

		static const DIR FIND_DIRS[to_underlying(DIR::MAX_COUNT)][5];
		static const int_t FIND_DIR_COUNTS[to_underlying(DIR::MAX_COUNT)];
		static const int_t FIND_LIMIT;
		static const float_t FIND_DEPTH;
		static const vec2 RESET_POS;

		Status& m_status;
		Scene& m_scene;
		const boost::optional<Item>& m_itemTarget;
		const boost::optional<Enemy>& m_enemyTarget;
		bool m_underEnemy;

		DIR m_dir;
		bool m_slowFirst;			// 是否慢速优先

		float_t m_bestScore;
		int_t m_count;
	};
}
