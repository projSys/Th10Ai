#pragma once

#include "Th10Ai/Common.h"

#include <memory>
#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <boost/optional.hpp>
#include <Base/Clock.h>

#if RENDER
#include <opencv2/opencv.hpp>
#endif

#include "Th10Ai/Console.h"
#include "Th10Ai/Status.h"
#include "Th10Ai/Scene.h"
#include "Th10Ai/Input.h"
#include "Th10Ai/Node.h"

namespace th
{
	class Th10Ai
	{
	public:
		Th10Ai(HWND window);
		~Th10Ai();

		void updateStatus();
		void commitInput(DWORD size, LPVOID data);

	private:
		void controlProc();
		void start();
		void stop();

		void handleProc();
		bool handle();
		bool handleBomb();
		bool handleTalk();
		bool handleShoot();
		bool handleMove();

		boost::optional<Item> findItem();
		boost::optional<Enemy> findEnemy();

		Console m_console;

		std::thread m_controlThread;
		std::atomic<bool> m_controlDone;
		std::thread m_handleThread;
		std::atomic<bool> m_handleDone;
		std::atomic<bool> m_active;

		std::mutex m_statusMutex;
		std::condition_variable m_statusCond;
		bool m_statusUpdated;
		std::unique_ptr<Status> m_writableStatus;
		std::unique_ptr<Status> m_intermediateStatus;
		std::unique_ptr<Status> m_readableStatus;
		Status m_status2;
		Status m_status1;
		Status m_status0;

		Scene m_scene2;
		Scene m_scene1;
		Scene m_scene;

		Time m_bombTime;
		int_t m_bombCount;
		Time m_findItemTime;

		std::mutex m_inputMutex;
		bool m_inputUpdated;
		std::unique_ptr<Input> m_writableInput;
		std::unique_ptr<Input> m_intermediateInput;
		std::unique_ptr<Input> m_readableInput;

		std::atomic<int_t> inputFrame;
		std::atomic<int_t> statusFrame;
		std::atomic<int_t> handleFrame;

		Node* m_root;

#if RENDER
		cv::Mat m_mat;
#endif
	};

	extern std::unique_ptr<Th10Ai> g_th10Ai;
}
