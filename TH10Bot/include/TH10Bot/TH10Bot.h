#pragma once

#include <vector>
#include <Base/Process.h>
#include <Base/Window.h>
#include <GraphCap/D3D9FrameSync.h>
#include <GraphCap/GraphCap.h>
#include <GraphCap/DesktopInput.h>

#include "TH10Bot/DesktopCapturerCV.h"
#include "TH10Bot/DesktopImageCV.h"
#include "TH10Bot/TH10Reader.h"
#include "TH10Bot/Player.h"
#include "TH10Bot/Item.h"
#include "TH10Bot/Enemy.h"
#include "TH10Bot/Bullet.h"
#include "TH10Bot/Laser.h"
#include "TH10Bot/Scene.h"
#include "TH10Bot/Clock.h"

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

	struct Result
	{
		bool valid;
		bool slow;		// 实际是否慢速
		float_t score;
		int_t size;
	};

	class TH10Bot
	{
	public:
		TH10Bot();
		~TH10Bot();

		void run();
		void start();
		void stop();
		void update();

	private:
		bool handleBomb();
		bool handleTalk();
		bool handleShoot();
		bool handleMove();
		Result dfs(const Action& action);
		bool isUnderEnemy();
		//NodeScore getNodeScore(const Player& player, float_t frame);
		int_t findItem();
		int_t findEnemy();
		float_t getCollectItemScore(const Player& player);
		float_t getShootEnemyScore(const Player& player);
		float_t getGobackScore(const Player& player);

		void move(Direction dir, bool slow);
		void pause();
		void resume();

		Process m_process;
		Window m_window;

		GraphCap m_graphCap;
		D3D9FrameSync m_frameSync;

		DesktopCapturerCV m_capturer;
		DesktopImageCV m_buffer;
		DesktopInput m_input;

		TH10Reader m_reader;

		bool m_active;
		bool m_pause;
		Clock m_clock;

		Player m_player;
		std::vector<Item> m_items;
		std::vector<Enemy> m_enemies;
		std::vector<Bullet> m_bullets;
		std::vector<Laser> m_lasers;

		Scene m_scene;

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
	};
}
