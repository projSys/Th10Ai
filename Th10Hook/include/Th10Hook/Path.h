#pragma once

#include "Th10Hook/Common.h"

#include "Th10Hook/Status.h"
#include "Th10Hook/Scene.h"

namespace th
{
	struct Action
	{
		Pointf fromPos;
		DIR fromDir;
		float_t frame;

		//int_t willCollideCount;
		//float_t minCollideFrame;
		float64_t score;
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
			const boost::optional<Enemy>& enemyTarget, bool underEnemy);

		Result find(DIR dir);
		Result dfs(const Action& action);

	//private:
		static float_t CalcFarScore(Pointf player, Pointf target);
		static float_t CalcNearScore(Pointf player, Pointf target);
		static float_t CalcShootScore(Pointf player, Pointf target);

		static const DIR FIND_DIRS[enum_cast(DIR::MAX_COUNT)][5];
		static const int_t FIND_DIR_COUNTS[enum_cast(DIR::MAX_COUNT)];
		static const int_t FIND_LIMIT;
		static const float_t FIND_DEPTH;
		static const Pointf RESET_POS;

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
