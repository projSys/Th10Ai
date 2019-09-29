#pragma once

#include "libTH10AI/ApiHook/D3D9Hook.h"
#include "libTH10AI/ApiHook/DI8Hook.h"
#include "libTH10AI/Api.h"
#include "libTH10AI/Data.h"
#include "libTH10AI/Scene.h"
#include "libTH10AI/Mover.h"

namespace th
{
	class HookThread;

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

	class Ai
	{
	public:
		Ai();
		~Ai();

		void run(HookThread& container);
		void notify();
		bool isKeyPressed(int vKey) const;

		void print();
		void start();
		void stop();
		void update();

		bool handleBomb();
		bool handleTalk();
		bool handleShoot();
		bool handleMove();
		Reward dfs(const Action& action);
		float_t calcCollectScore(const Player& player);
		float_t calcShootScore(const Player& player);
		float_t calcGobackScore(const Player& player);

		void move(Direction dir, bool slow);

	private:
		D3D9Hook m_d3d9Hook;
		DI8Hook m_di8Hook;
		Api m_api;
		Data m_data;

		Scene m_scene;

		bool m_active;
		float_t m_bestScore;
		Direction m_bestDir;
		bool m_bestSlow;
		int_t m_count;
		int_t m_limit;

		int_t m_itemId;
		int_t m_enemyId;

		int_t m_bombCount;
		Direction m_prevDir;
		bool m_prevSlow;
	};
}
