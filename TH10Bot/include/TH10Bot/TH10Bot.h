#pragma once

#include <vector>
#include <set>
#include <sstream>
#include <Windows/Process.h>
#include <Windows/Window.h>
#include <GraphCap/D3D9FrameSync.h>
#include <GraphCap/GraphCap.h>

#include "TH10Bot/DesktopCapturerCV.h"
#include "TH10Bot/DesktopImageCV.h"
#include "TH10Bot/TH10Reader.h"
#include "TH10Bot/Entity.h"
#include "TH10Bot/Scene.h"
#include "TH10Bot/Key.h"
#include "TH10Bot/Clock.h"

namespace th
{
	struct BulletView
	{
		uint_t index;
		Direction dir;

		FootPoint footPoint;
	};

	struct LaserLv1
	{
		uint_t index;
		float_t distance;
		FootPoint footPoint;
		float_t angle;
		Direction dir;
	};

	struct Node
	{
		float_t gScore;
		float_t hScore;
		float_t fScore;
	};

	struct PositionLess
	{
		bool operator ()(const Node& left, const Node& right) const
		{
			return false;
		}
	};

	struct ScoreLess
	{
		bool operator ()(const Node& left, const Node& right) const
		{
			return left.fScore < right.fScore;
		}
	};

	struct NodeScore
	{
		float_t score;
		float_t minCollideFrame;
		Direction minCollideDir;
		int_t willCollideCount;
	};

	struct NodeResult
	{
		float_t score;
		Direction nextDir;
		float_t minCollideFrame;
	};

	class TH10Bot
	{
	public:
		TH10Bot();
		~TH10Bot();

		void start();
		void stop();
		void update();

	private:
		static float_t GetYFactor(const Pointf& source, const Pointf& next);
		static float_t GetDistFactor(float_t source, float_t next, float_t target);
		static float_t GetDistXScore(float_t xNext, float_t xTarget);
		static float_t GetDistYScore(float_t yNext, float_t yTarget);

		bool handleBomb();
		//bool collideBomb();

		bool handleTalk();

		bool handleShoot();

		bool handleMove();
		NodeResult dfs(const Player& player, float_t frame, int_t depth);
		void aStar(Node& start, Node& goal);
		float_t distBetween(const Node& current, const Node& neighbor);
		float_t heuristicCostEstimate(const Node& neighbor, const Node& goal);
		void reconstructPath(const Node& goal);
		NodeScore getNodeScore(const Player& player, float_t frame);
		bool collideMove(const Player& player, float_t frame);
		float_t getTargetScore(const Player& pNext, const Pointf& target);
		int_t findItem();
		int_t findEnemy();
		float_t getDodgeEnemyScore(const Player& pNext, float_t epsilon = 100.0);
		float_t getDodgeLaserScore(const Player& pNext, float_t epsilon = 2.0);
		float_t getCollectItemScore(const Player& pNext, int_t itemId);
		float_t getShootEnemyScore(const Player& pNext, int_t enemyId);
		float_t getGobackScore(const Player& pNext);

		static bool IsSlow(Direction dir);
		void move(Direction dir);

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
		DesktopImageCV m_image;
		TH10Reader m_reader;

		bool m_active;
		Clock m_clock;
		Key m_keyUp, m_keyDown, m_keyLeft, m_keyRight, m_keyShift, m_keyZ, m_keyX;

		Player m_player;
		std::vector<Item> m_items;
		std::vector<Enemy> m_enemies;
		std::vector<Bullet> m_bullets;
		std::vector<Laser> m_lasers;

		std::vector<BulletView> m_focusBullets;
		std::vector<LaserLv1> m_focusLasers;

		int_t m_itemId;
		int_t m_enemyId;

		std::vector<Node> m_path;

		time_t m_bombCooldown;
		time_t m_talkCooldown;
		time_t m_shootCooldown;
		time_t m_collectCooldown;
	};
}
