#pragma once

#include "libTH10AI/Data.h"
#include "libTH10AI/Scene.h"

namespace th
{
	struct Action
	{
		Pointf fromPos;
		Direction fromDir;
		bool slowFirst;			// 是否慢速优先
		float_t frame;
	};

	struct Result
	{
		bool valid;
		bool slow;		// 实际是否慢速
		float_t score;
		int_t size;
	};

	class Path
	{
	public:
		Path(Data& data, Scene& scene, Direction pathDir,
			const ItemTarget& itemTarget, const EnemyTarget& enemyTarget);

		Result find(bool underEnemy);
		Result dfs(const Action& action);
		float_t calcCollectScore(const Player& player);
		float_t calcShootScore(const Player& player);
		float_t calcGobackScore(const Player& player);

	//private:
		Data& m_data;
		Scene& m_scene;
		Direction m_pathDir;
		ItemTarget m_itemTarget;
		EnemyTarget m_enemyTarget;

		float_t m_bestScore;
		Direction m_bestDir;
		bool m_bestSlow;
		int_t m_count;
		int_t m_limit;
	};
}
