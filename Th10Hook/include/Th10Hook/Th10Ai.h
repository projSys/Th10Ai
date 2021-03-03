#pragma once

#include "Th10Hook/Common.h"

#include <atomic>
#include <mutex>
#include <condition_variable>

#include "Th10Hook/WindowHook.h"
#include "Th10Hook/DirectX/D3D9Hook.h"
#include "Th10Hook/DirectX/DI8Hook.h"
#include "Th10Hook/Th10Hook.h"
#include "Th10Hook/Th10Types.h"
#include "Th10Hook/Status.h"
#include "Th10Hook/Scene.h"

namespace th
{
	class Th10Ai :
		public WindowListener,
		public D3D9Listener,
		public DI8Listener,
		public Th10Listener
	{
	public:
		Th10Ai();
		~Th10Ai();

		void run();

	private:
		virtual void onHook() override;
		virtual void onUnhook() override;
		virtual void onDestroy() override;
		virtual void onPresent(IDirect3DDevice9* device, const RECT* sourceRect, const RECT* destRect,
			HWND destWindowOverride, const RGNDATA* dirtyRegion) override;
		virtual void onGetDeviceStateA(IDirectInputDevice8A* device, DWORD size, LPVOID data) override;

		void start();
		void stop();

		bool handle();
		bool handleBomb();
		bool handleTalk();
		bool handleShoot();
		bool handleMove();
		void move(DIR dir, bool slow);

		std::unique_ptr<D3D9Hook> m_d3d9Hook;
		std::unique_ptr<DI8Hook> m_di8Hook;
		std::unique_ptr<Th10Hook> m_th10Hook;

		std::atomic<bool> m_done;
		bool m_active;
		int64_t m_bombTime;
		int_t m_bombCount;

		StatusData m_statusData;
		ActionData m_actionData;
		std::atomic<bool> m_actionUpdated;

		Status m_status2;
		Status m_status1;
		Status m_status;

		Scene m_scene;

		std::mutex m_updateMutex;
		std::condition_variable m_updateCond;
		bool m_updated;
	};
}
