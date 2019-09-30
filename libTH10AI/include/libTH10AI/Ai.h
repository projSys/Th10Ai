#pragma once

#include "libTH10AI/ApiHook/D3D9Hook.h"
#include "libTH10AI/ApiHook/DI8Hook.h"
#include "libTH10AI/Api.h"
#include "libTH10AI/Data.h"
#include "libTH10AI/Scene.h"

namespace th
{
	class HookThread;

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

		void move(Direction dir, bool slow);

	private:
		D3D9Hook m_d3d9Hook;
		DI8Hook m_di8Hook;
		Api m_api;
		Data m_data;

		Scene m_scene;

		bool m_active;

		int_t m_itemId;
		int_t m_enemyId;

		int_t m_bombCount;
		Direction m_prevDir;
		bool m_prevSlow;
	};
}
