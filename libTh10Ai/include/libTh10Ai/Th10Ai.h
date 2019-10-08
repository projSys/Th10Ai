#pragma once

#include <thread>

#include "libTh10Ai/D3D9Hook.h"
#include "libTh10Ai/DI8Hook.h"
#include "libTh10Ai/Reader.h"
#include "libTh10Ai/Data.h"
#include "libTh10Ai/Scene.h"

namespace th
{
	class HookThread;

	class Th10Ai
	{
	public:
		Th10Ai();
		~Th10Ai();

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
		bool m_active;
		int_t m_bombCount;
		Direction m_prevDir;
		bool m_prevSlow;

		D3D9Hook m_d3d9Hook;
		DI8Hook m_di8Hook;
		Reader m_reader;
		Data m_data;
		Scene m_scene;
	};
}
