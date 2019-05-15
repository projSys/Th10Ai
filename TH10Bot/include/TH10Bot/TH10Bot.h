#pragma once

#include <vector>
#include <Windows/Process.h>
#include <Windows/Window.h>
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
		static float_t GetYFactor(const Pointf& source, const Pointf& next);
		static float_t GetDistFactor(float_t source, float_t next, float_t target);
		static float_t GetDistXScore(float_t xNext, float_t xTarget);
		static float_t GetDistYScore(float_t yNext, float_t yTarget);

		bool handleBomb();

		bool handleTalk();

		bool handleShoot();

		bool handleMove();
		Pointf getMousePos();
		NodeScore dfs(const Node& node);
		//void astar(Node& start, Node& goal);
		//float_t distBetween(const Node& current, const Node& neighbor);
		//float_t heuristicCostEstimate(const Node& neighbor, const Node& goal);
		//void reconstructPath(const PointNodeMap& closedSet, const Node& goal);
		NodeScore getNodeScore(const Player& player, float_t frame);
		int_t findItem();
		int_t findEnemy();
		float_t getCollectItemScore(const Player& pNext, int_t itemId);
		float_t getShootEnemyScore(const Player& pNext, int_t enemyId);
		float_t getGobackScore(const Player& pNext);

		void move(Direction dir, bool slow);

		// Item
		//bool handleItem();
		//bool checkCollectStatus();
		//bool collectItem(int_t itemId);
		//float_t collectItemScore(const Player& next, const Item& item);

		// Enemy
		//bool handleEnemy();
		//bool checkShootStatus();
		//bool shootEnemy(int_t enemyId);
		//float_t dodgeEnemyScore(const Player& next);
		//float_t shootEnemyScore(const Player& next, const Enemy& enemy);

		//void goback();

		Process m_process;
		Window m_window;

		D3D9FrameSync m_sync;
		GraphCap m_graphCap;

		DesktopCapturerCV m_capturer;
		DesktopImageCV m_buffer;
		DesktopInput m_input;

		TH10Reader m_reader;

		bool m_active;
		Clock m_clock;

		Player m_player;
		std::vector<Item> m_items;
		std::vector<Enemy> m_enemies;
		std::vector<Bullet> m_bullets;
		std::vector<Laser> m_lasers;

		Scene m_scene;

		std::vector<Node> m_path;
		//uint8_t m_mask[480][640];
		int_t m_count;

		int_t m_itemId;
		int_t m_enemyId;

		time_t m_bombCooldown;
		time_t m_talkCooldown;
		time_t m_shootCooldown;
		time_t m_collectCooldown;
	};
}
