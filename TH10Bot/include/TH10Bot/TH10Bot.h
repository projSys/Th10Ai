#pragma once

#include <vector>
#include <sstream>

#include <Windows/Process.h>
#include <Windows/Window.h>
#include <GraphCap/D3D9FrameSync.h>

#include "TH10Bot/DesktopCapturerCV.h"
#include "TH10Bot/DesktopImageCV.h"
#include "TH10Bot/TH10Reader.h"
#include "TH10Bot/Entity.h"
#include "TH10Bot/Define.h"
#include "TH10Bot/Key.h"
#include "TH10Bot/Clock.h"

namespace th
{
	struct Node
	{
		int depth;
		int parentId;
		Player player;
		uint8_t dir;
		bool slow;
		double score;
	};

	const int DEPTH = 4;

	class TH10Bot
	{
	public:
		TH10Bot();
		~TH10Bot();

		void start();
		void stop();
		void setSlowManual(bool slowManual);
		void update();

	private:
		static bool IsInScene(double x, double y);
		static void FixPos(double& x, double& y);

		static double GetYFactor(const Rect2d& source, const Rect2d& next);
		static double GetDistFactor(double source, double next, double target);
		static double GetDistXScore(double xNext, double xTarget);
		static double GetDistYScore(double yNext, double yTarget);

		bool handleBomb();
		bool hitTestBomb();

		bool handleTalk();

		bool handleShoot();

		bool handleMove();
		bool hitTestMove(const Player& player);
		double getTargetScore(const Player& pNext, const Rect2d& target);
		double search(const Player& player, int depth);
		bool hitTestMove(const Player& player, int depth);
		int findPower();
		int findEnemy();
		double getDodgeEnemyScore(const Player& pNext, double epsilon = 100.0);
		double getDodgeBulletScore(const Player& pNext, double epsilon = 2.0);
		double getDodgeLaserScore(const Player& pNext, double epsilon = 2.0);
		double getBulletAngleScore(const Player& pNext);
		double getPickupPowerScore(const Player& pNext, int powerId);
		double getShootEnemyScore(const Player& pNext, int enemyId);
		double getGobackScore(const Player& pNext);
		void move(int dir, bool slow);

		// Power
		//bool handlePower();
		//bool checkPickupStatus();
		//bool pickupPower(int powerId);
		//double pickupPowerScore(const Player& next, const Power& power);

		// Enemy
		//bool handleEnemy();
		//bool checkShootStatus();
		//bool shootEnemy(int enemyId);
		//double dodgeEnemyScore(const Player& next);
		//double shootEnemyScore(const Player& next, const Enemy& enemy);

		//void goback();

		Process m_process;
		Window m_window;
		//D3D9FrameSync m_sync;
		DesktopCapturerCV m_capturer;
		DesktopImageCV m_image;
		TH10Reader m_reader;

		bool m_active;
		Clock m_clock;
		Key m_keyUp, m_keyDown, m_keyLeft, m_keyRight, m_keyShift, m_keyZ, m_keyX;
		bool m_slowManual;

		Player m_player;
		std::vector<Item> m_items;
		std::vector<Enemy> m_enemies;
		std::vector<Bullet> m_bullets;
		std::vector<Laser> m_lasers;

		std::vector<std::vector<Bullet>> m_depthList;
		std::vector<std::vector<Bullet>> m_clipList;

		time_t m_bombCooldown;
		time_t m_talkCooldown;
		time_t m_shootCooldown;
		time_t m_pickupCooldown;

		std::ostringstream m_log;
	};
}
