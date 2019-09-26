#pragma once

#include "libTH10Bot/ApiHook/D3D9Hook.h"
#include "libTH10Bot/ApiHook/DI8Hook.h"
#include "libTH10Bot/Api.h"
#include "libTH10Bot/Data.h"
#include "libTH10Bot/Scene.h"
#include "libTH10Bot/Clock.h"
#include "libTH10Bot/Mover.h"

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

	class Bot
	{
	public:
		Bot();
		~Bot();

		void run(HookThread& container);
		void notify();
		bool isKeyPressed(int vKey) const;

		void start();
		void stop();
		void update();

		bool handleBomb();
		bool handleTalk();
		bool handleShoot();
		bool handleMove();
		Reward dfs(const Action& action);
		//NodeScore calcNodeScore(const Player& player, float_t frame);
		int_t findItem();
		int_t findEnemy();
		float_t calcCollectScore(const Player& player);
		float_t calcShootScore(const Player& player);
		float_t calcGobackScore(const Player& player);

		void move(Direction dir, bool slow);

	private:
		D3D9Hook m_d3d9Hook;
		DI8Hook m_di8Hook;

		Api m_api;
		Data m_data;

		Clock m_clock;

		Scene m_scene;

		bool m_active;
		float_t m_bestScore;
		Direction m_bestDir;
		bool m_bestSlow;
		int_t m_count;
		int_t m_limit;

		int_t m_itemId;
		int_t m_enemyId;

		time_t m_bombCooldown;
		time_t m_talkCooldown;
		time_t m_collectCooldown;

		int_t m_bombCount;
	};
}
