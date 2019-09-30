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
		Direction targetDir;
	};

	struct Reward
	{
		bool valid;
		bool slow;		// 实际是否慢速
		float_t score;
		int_t size;
	};

	class Path
	{
	public:
		Path(Data& data, Scene& scene, int_t itemId, int_t enemyId);

		Reward find(Direction dir, bool underEnemy);
		Reward dfs(const Action& action);
		float_t calcCollectScore(const Player& player);
		float_t calcShootScore(const Player& player);
		float_t calcGobackScore(const Player& player);

	//private:
		Data m_data;
		Scene m_scene;
		int_t m_itemId;
		int_t m_enemyId;

		float_t m_bestScore;
		Direction m_bestDir;
		bool m_bestSlow;
		int_t m_count;
		int_t m_limit;
	};
}
