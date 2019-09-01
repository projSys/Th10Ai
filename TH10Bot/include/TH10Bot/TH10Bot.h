#pragma once

#include <Windows/Process.h>
#include <Windows/Window.h>
#include <GraphCap/GraphCap.h>
#include <GraphCap/D3D9FrameSync.h>
#include <GraphCap/DesktopInput.h>
#include <GraphCap/Direct3D.h>

#include "TH10Bot/DesktopCapturerCV.h"
#include "TH10Bot/DesktopImageCV.h"
#include "TH10Bot/Th10Api.h"
#include "TH10Bot/Th10Data.h"
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

	struct Reward
	{
		bool valid;
		bool slow;		// 实际是否慢速
		float_t score;
		int_t size;
	};

	class Th10Bot
	{
	public:
		Th10Bot();
		~Th10Bot();

		bool isRunning();
		void start();
		void stop();
		void quit();
		void run();
		void update();

	private:
		bool handleBomb();
		bool handleTalk();
		bool handleShoot();
		bool handleMove();
		Reward dfs(const Action& action);
		bool isUnderEnemy();
		//NodeScore calcNodeScore(const Player& player, float_t frame);
		int_t findItem();
		int_t findEnemy();
		float_t calcCollectScore(const Player& player);
		float_t calcShootScore(const Player& player);
		float_t calcGobackScore(const Player& player);

		void move(Direction dir, bool slow);

		bool m_active;

		Process m_process;
		Window m_window;
		GraphCap m_graphCap;
		D3D9FrameSync m_frameSync;
		DesktopInput m_input;
		Direct3D m_d3d;
		DesktopCapturerCV m_capturer;
		DesktopImageCV m_buffer;

		Th10Api m_api;
		Th10Data m_data;

		Clock m_clock;

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
