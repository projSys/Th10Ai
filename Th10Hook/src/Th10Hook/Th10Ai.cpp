#include "Th10Hook/Th10Ai.h"

#include <dinput.h>
#include <boost/optional.hpp>
#include <Base/Time.h>
#include <Base/Windows/Apis.h>

#include "Th10Hook/Path.h"

namespace th
{
	std::unique_ptr<Th10Ai> g_th10Ai;

	Th10Ai::Th10Ai(HWND window) :
		m_controlDone(false),
		m_handleDone(false),
		m_active(false),
		m_bombTime(0),
		m_bombCount(0),
		m_actionUpdated(false)
	{
		SetForegroundWindow(window);

		RECT rect = {};
		GetWindowRect(window, &rect);
		int screenWidth = GetSystemMetrics(SM_CXSCREEN);
		int screenHeight = GetSystemMetrics(SM_CYSCREEN);
		int x = (screenWidth - (rect.right - rect.left)) / 2;
		int y = (screenHeight - (rect.bottom - rect.top)) / 2;
		SetWindowPos(window, nullptr, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

		char buf1[1024] = {};
		GetWindowTextA(window, buf1, 1023);
		std::wstring buf2 = Apis::MultiByteToWideChar(932, buf1, -1);	// Shift-JIS
		std::string buf3 = Apis::WideToAnsi(buf2);
		SetWindowTextA(window, buf3.c_str());

		//ULONG_PTR icon = GetClassLongPtrA(window, GCLP_HICON);
		HICON icon = LoadIconA(GetModuleHandleA(nullptr), "IDI_ICON3");
		SendMessageA(window, WM_SETICON, ICON_SMALL, (LPARAM)icon);

		m_scene.split(6);

		m_controlThread = std::thread(&Th10Ai::controlProc, this);
		m_handleThread = std::thread(&Th10Ai::handleProc, this);
	}

	Th10Ai::~Th10Ai()
	{
		m_handleDone = true;
		m_active = false;
		{
			std::unique_lock<std::mutex> lock(m_statusMutex);
			m_statusUpdated = true;
			m_statusCond.notify_one();
		}
		if (m_handleThread.joinable())
			m_handleThread.join();

		m_controlDone = true;
		if (m_controlThread.joinable())
			m_controlThread.join();
	}

	bool IsKeyPressed(int vKey)
	{
		return (GetAsyncKeyState(vKey) & 0x8000) != 0;
	}

	void Th10Ai::controlProc()
	{
		std::cout << "保持焦点在游戏窗口上，开始游戏，然后按A开启AI，按S停止AI。" << std::endl;
		while (!m_controlDone)
		{
			if (IsKeyPressed('A'))
			{
				start();
			}
			else if (IsKeyPressed('S'))
			{
				stop();
			}
			else
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(16));
			}
		}
	}

	void Th10Ai::start()
	{
		if (!m_active)
		{
			m_active = true;
			std::cout << "开启AI。" << std::endl;
		}
	}

	void Th10Ai::stop()
	{
		if (m_active)
		{
			m_active = false;
			std::cout << "停止AI。" << std::endl;
			std::cout << "决死总数：" << m_bombCount << std::endl;
		}
	}

	void Th10Ai::handleProc()
	{
		while (!m_handleDone)
		{
			if (m_active)
			{
				handle();
			}
			else
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(16));
			}
		}
	}

	void Th10Ai::updateStatus()
	{
		if (!m_active)
			return;

		m_status.update();

		std::unique_lock<std::mutex> lock(m_statusMutex);
		m_statusUpdated = true;
		m_statusCond.notify_one();
	}

	void Th10Ai::commitAction(DWORD size, LPVOID data)
	{
		if (!m_active)
			return;

		m_status.m_inputFrame += 1;

		// c_dfDIKeyboard
		if (size == 256 && data != nullptr)
		{
			//lock_guard<mutex> lock(m_keyMutex);
			if (m_actionUpdated)
			{
				m_actionUpdated = false;

				BYTE* keyState = reinterpret_cast<BYTE*>(data);

				if (m_actionData.left)
					keyState[DIK_LEFT] = 0x80;
				else
					keyState[DIK_LEFT] = 0x00;

				if (m_actionData.right)
					keyState[DIK_RIGHT] = 0x80;
				else
					keyState[DIK_RIGHT] = 0x00;

				if (m_actionData.up)
					keyState[DIK_UP] = 0x80;
				else
					keyState[DIK_UP] = 0x00;

				if (m_actionData.down)
					keyState[DIK_DOWN] = 0x80;
				else
					keyState[DIK_DOWN] = 0x00;

				if (m_actionData.shoot)
					keyState[DIK_Z] = 0x80;
				else
					keyState[DIK_Z] = 0x00;

				if (m_actionData.bomb)
					keyState[DIK_X] = 0x80;
				else
					keyState[DIK_X] = 0x00;

				if (m_actionData.slow)
					keyState[DIK_LSHIFT] = 0x80;
				else
					keyState[DIK_LSHIFT] = 0x00;

				if (m_actionData.skip)
					keyState[DIK_LCONTROL] = 0x80;
				else
					keyState[DIK_LCONTROL] = 0x00;
			}
			else
			{
				std::cout << "动作提交太慢了。" << std::endl;
			}
		}
		// c_dfDIMouse
		//sizeof(DIMOUSESTATE);
		// c_dfDIMouse2
		//sizeof(DIMOUSESTATE2);
		// c_dfDIJoystick
		//sizeof(DIJOYSTATE);
		// c_dfDIJoystick2
		//sizeof(DIJOYSTATE2);
	}

	bool Th10Ai::handle()
	{
		{
			std::unique_lock<std::mutex> lock(m_statusMutex);
			if (!m_statusUpdated)
				m_statusCond.wait(lock);
			else
				std::cout << "状态更新太慢了。" << std::endl;
			m_statusUpdated = false;

			m_status2.copy(m_status1);
			m_status1.copy(m_status0);
			m_status0.copy(m_status);
		}

		if (!m_active)
			return false;

		m_scene.clearAll();
		m_scene.splitEnemies(m_status0.getEnemies());
		m_scene.splitBullets(m_status0.getBullets());
		m_scene.splitLasers(m_status0.getLasers());

		handleBomb();
		handleTalk();
		handleShoot();
		handleMove();

		m_actionUpdated = true;

		return true;
	}

	// 处理炸弹
	bool Th10Ai::handleBomb()
	{
		if (m_status0.getPlayer().isColliding())
		{
			Time time = Time::Now();
			int64_t now = time.getMilliSeconds();
			if (now - m_bombTime > 1000)
			{
				m_bombTime = now;

				int_t id = m_status0.collide(m_status0.getPlayer(), 0.0);
				m_status1.collide(m_status0.getPlayer(), 1.0);
				m_status2.collide(m_status0.getPlayer(), 2.0);
				m_status1.collide(m_status0.getPlayer(), 1.0, id);
				m_status2.collide(m_status0.getPlayer(), 2.0, id);

				m_actionData.bomb = true;
				m_bombCount += 1;
				std::cout << "决死：" << m_bombCount << std::endl;
				return true;
			}
			else
			{
				m_actionData.bomb = false;
				return false;
			}
		}
		else
		{
			m_actionData.bomb = false;
			return false;
		}
	}

	// 处理对话
	bool Th10Ai::handleTalk()
	{
		if (m_status0.isTalking())
		{
			m_actionData.skip = true;
			return true;
		}
		else
		{
			m_actionData.skip = false;
			return false;
		}
	}

	// 处理攻击
	bool Th10Ai::handleShoot()
	{
		if (m_status0.hasEnemy())
		{
			m_actionData.shoot = true;
			return true;
		}
		else
		{
			m_actionData.shoot = false;
			return false;
		}
	}

	// 处理移动
	bool Th10Ai::handleMove()
	{
		if (!m_status0.getPlayer().isNormalStatus())
			return false;

		boost::optional<Item> itemTarget = m_status0.findItem();
		boost::optional<Enemy> enemyTarget = m_status0.findEnemy();
		bool underEnemy = m_status0.isUnderEnemy();

		float_t bestScore = std::numeric_limits<float_t>::lowest();
		boost::optional<DIR> bestDir;
		boost::optional<bool> bestSlow;

		for (DIR dir : DIRS)
		{
			Path path(m_status0, m_scene, itemTarget, enemyTarget, underEnemy);
			Result result = path.find(dir);

			if (result.valid && path.m_bestScore > bestScore)
			{
				bestScore = path.m_bestScore;
				bestDir = path.m_dir;
				bestSlow = result.slow;
			}
		}

		if (bestDir.has_value() && bestSlow.has_value())
		{
			move(bestDir.value(), bestSlow.value());
		}
		else
		{
			move(DIR::HOLD, false);
			std::cout << "无路可走。" << std::endl;
		}

		return true;
	}

	void Th10Ai::move(DIR dir, bool slow)
	{
		if (slow)
			m_actionData.slow = true;
		else
			m_actionData.slow = false;

		switch (dir)
		{
		case DIR::HOLD:
			m_actionData.left = false;
			m_actionData.right = false;
			m_actionData.up = false;
			m_actionData.down = false;
			break;

		case DIR::LEFT:
			m_actionData.left = true;
			m_actionData.right = false;
			m_actionData.up = false;
			m_actionData.down = false;
			break;

		case DIR::RIGHT:
			m_actionData.left = false;
			m_actionData.right = true;
			m_actionData.up = false;
			m_actionData.down = false;
			break;

		case DIR::UP:
			m_actionData.left = false;
			m_actionData.right = false;
			m_actionData.up = true;
			m_actionData.down = false;
			break;

		case DIR::DOWN:
			m_actionData.left = false;
			m_actionData.right = false;
			m_actionData.up = false;
			m_actionData.down = true;
			break;

		case DIR::LEFTUP:
			m_actionData.left = true;
			m_actionData.right = false;
			m_actionData.up = true;
			m_actionData.down = false;
			break;

		case DIR::RIGHTUP:
			m_actionData.left = false;
			m_actionData.right = true;
			m_actionData.up = true;
			m_actionData.down = false;
			break;

		case DIR::LEFTDOWN:
			m_actionData.left = true;
			m_actionData.right = false;
			m_actionData.up = false;
			m_actionData.down = true;
			break;

		case DIR::RIGHTDOWN:
			m_actionData.left = false;
			m_actionData.right = true;
			m_actionData.up = false;
			m_actionData.down = true;
			break;
		}
	}
}
