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
#include "TH10Bot/Scene.h"
#include "TH10Bot/Key.h"
#include "TH10Bot/Clock.h"

namespace th
{
	struct Node
	{
		int_t depth;
		int_t parentId;
		Player player;
		int_t dir;
		bool slow;
		float_t score;
	};

	const int_t DEPTH = 4;

	struct BulletLv1
	{
		BulletLv1(uint_t index0, float_t distance0, float_t footFrame0, const Pointf& footPoint0, float_t angle0) :
			index(index0), distance(distance0), footFrame(footFrame0), footPoint(footPoint0), angle(angle0) {}

		uint_t index;
		float_t distance;
		float_t footFrame;
		Pointf footPoint;
		float_t angle;
	};

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
		static float_t GetYFactor(const Pointf& source, const Pointf& next);
		static float_t GetDistFactor(float_t source, float_t next, float_t target);
		static float_t GetDistXScore(float_t xNext, float_t xTarget);
		static float_t GetDistYScore(float_t yNext, float_t yTarget);

		bool handleBomb();
		bool hitTestBomb();

		bool handleTalk();

		bool handleShoot();

		bool handleMove();
		Pointf getMousePos();
		bool hitTestMove(const Player& player);
		float_t getTargetScore(const Player& pNext, const Pointf& target);
		float_t search(const Player& player, int_t depth);
		bool hitTestMove(const Player& player, int_t depth);
		int_t findPower();
		int_t findEnemy();
		float_t getDodgeEnemyScore(const Player& pNext, float_t epsilon = 100.0);
		float_t getDodgeBulletScore(const Player& pNext, float_t epsilon = 2.0);
		float_t getDodgeLaserScore(const Player& pNext, float_t epsilon = 2.0);
		float_t getBulletAngleScore(const Player& pNext);
		float_t getPickupPowerScore(const Player& pNext, int_t powerId);
		float_t getShootEnemyScore(const Player& pNext, int_t enemyId);
		float_t getGobackScore(const Player& pNext);
		void move(int_t dir, bool slow);

		// Power
		//bool handlePower();
		//bool checkPickupStatus();
		//bool pickupPower(int_t powerId);
		//float_t pickupPowerScore(const Player& next, const Power& power);

		// Enemy
		//bool handleEnemy();
		//bool checkShootStatus();
		//bool shootEnemy(int_t enemyId);
		//float_t dodgeEnemyScore(const Player& next);
		//float_t shootEnemyScore(const Player& next, const Enemy& enemy);

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

		std::vector<BulletLv1> m_redList;
		std::vector<BulletLv1> m_yellowList;

		//std::vector<std::vector<Bullet>> m_depthList;
		//std::vector<std::vector<Bullet>> m_clipList;

		time_t m_bombCooldown;
		time_t m_talkCooldown;
		time_t m_shootCooldown;
		time_t m_pickupCooldown;

		//std::ostringstream m_log;
	};
}
