#include "Th10Hook/Th10Ai.h"

#include <thread>
#include <boost/optional.hpp>
#include <Base/ScopeGuard.h>
#include <Base/Time.h>
#include <Base/Windows/Process.h>
#include <Base/Windows/Module.h>

#include "Th10Hook/Path.h"

namespace th
{
	Th10Ai::Th10Ai() :
		m_done(false),
		m_active(false),
		m_bombTime(0),
		m_bombCount(0),
		m_actionUpdated(false)
	{
		AllocConsole();
		freopen("conin$", "r", stdin);
		freopen("conout$", "w", stdout);
		freopen("conout$", "w", stderr);
		//system("chcp 65001");

		HWND console = GetConsoleWindow();
		HMENU menu = GetSystemMenu(console, FALSE);
		EnableMenuItem(menu, SC_CLOSE, MF_GRAYED | MF_BYCOMMAND);

		m_scene.split(6);
	}

	Th10Ai::~Th10Ai()
	{
	}

	bool IsKeyPressed(int vKey)
	{
		return (GetAsyncKeyState(vKey) & 0x8000) != 0;
	}

	void Th10Ai::run()
	{
		HWND window = FindWindowW(L"BASE", nullptr);//L"�������_�^�@�` Mountain of Faith. ver 1.00a");
		if (window == nullptr)
			BASE_THROW(Exception(u8"��������¼δ���С�"));

		// �ᴥ�����������麯���ص�����Ҫ���ڹ��캯��
		WindowHook& windowHook = WindowHook::GetInstance();
		windowHook.hook(window, this);
		ON_SCOPE_EXIT([&]()
			{
				windowHook.unhook();
			});

		std::cout << "�뽫������ڷ���¼�����ϣ���ʼ��Ϸ��Ȼ��A����AI����SֹͣAI����D�˳�AI��" << std::endl;
		while (!m_done)
		{
			if (IsKeyPressed('A'))
			{
				start();
			}
			else if (IsKeyPressed('S'))
			{
				stop();
			}
			else if (IsKeyPressed('D'))
			{
				stop();
				break;
			}
			else if (!handle())
			{
				stop();
				break;
			}
		}
		std::cout << "�˳�AI��" << std::endl;
	}

	void Th10Ai::onHook()
	{
		m_th10Hook = std::make_unique<Th10Hook>(this);
		m_d3d9Hook = std::make_unique<D3D9Hook>(this);
		m_di8Hook = std::make_unique<DI8Hook>(this);
	}

	void Th10Ai::onUnhook()
	{
		m_di8Hook = nullptr;
		m_d3d9Hook = nullptr;
		m_th10Hook = nullptr;
	}

	void Th10Ai::onDestroy()
	{
		m_done = true;
	}

	void Th10Ai::onPresent(IDirect3DDevice9* device, const RECT* sourceRect, const RECT* destRect,
		HWND destWindowOverride, const RGNDATA* dirtyRegion)
	{
		m_statusData.presentFrame += 1;
		m_th10Hook->readPlayer(m_statusData.player);
		m_statusData.itemCount = m_th10Hook->readItems(m_statusData.items);
		m_statusData.enemyCount = m_th10Hook->readEnemies(m_statusData.enemies);
		m_statusData.bulletCount = m_th10Hook->readBullets(m_statusData.bullets);
		m_statusData.laserCount = m_th10Hook->readLasers(m_statusData.lasers);

		std::unique_lock<std::mutex> lock(m_updateMutex);
		m_updated = true;
		m_updateCond.notify_one();
	}

	void Th10Ai::onGetDeviceStateA(IDirectInputDevice8A* device, DWORD size, LPVOID data)
	{
		m_statusData.inputFrame += 1;

		// c_dfDIKeyboard
		if (size == 256 && data != nullptr)
		{
			//lock_guard<mutex> lock(m_keyMutex);
			if (m_actionUpdated)
			{
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

				m_actionUpdated = false;
			}
			else
			{
				std::cout << "����̫���ˡ�" << std::endl;
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

	void Th10Ai::start()
	{
		if (!m_active)
		{
			m_active = true;
			std::cout << "����AI��" << std::endl;
		}
	}

	void Th10Ai::stop()
	{
		if (m_active)
		{
			m_active = false;
			std::cout << "ֹͣAI��" << std::endl;
			std::cout << "����������" << m_bombCount << std::endl;
		}
	}

	bool Th10Ai::handle()
	{
		if (!m_active)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(16));
			return true;
		}

		{
			std::unique_lock<std::mutex> lock(m_updateMutex);
			if (!m_updated)
				m_updateCond.wait(lock);
		}

		m_status2.update(m_status1);
		m_status1.update(m_status);
		m_status.update(m_statusData);

		m_scene.clearAll();
		m_scene.splitEnemies(m_status.getEnemies());
		m_scene.splitBullets(m_status.getBullets());
		m_scene.splitLasers(m_status.getLasers());

		handleBomb();
		handleTalk();
		handleShoot();
		handleMove();

		m_actionUpdated = true;

		return true;
	}

	// ����ը��
	bool Th10Ai::handleBomb()
	{
		if (m_status.getPlayer().isColliding())
		{
			Time time = Time::Now();
			int64_t now = time.getMilliSeconds();
			if (now - m_bombTime > 1000)
			{
				m_bombTime = now;

				int_t id = m_status.collide(m_status.getPlayer(), 0.0);
				m_status1.collide(m_status.getPlayer(), 1.0);
				m_status1.collide(m_status.getPlayer(), 2.0);
				m_status1.collide(m_status.getPlayer(), 1.0, id);
				m_status1.collide(m_status.getPlayer(), 2.0, id);

				m_actionData.bomb = true;
				m_bombCount += 1;
				std::cout << "������" << m_bombCount << std::endl;
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

	// ����Ի�
	bool Th10Ai::handleTalk()
	{
		if (m_status.isTalking())
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

	// ������
	bool Th10Ai::handleShoot()
	{
		if (m_status.hasEnemy())
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

	// �����ƶ�
	bool Th10Ai::handleMove()
	{
		if (!m_status.getPlayer().isNormalStatus())
			return false;

		boost::optional<Item> itemTarget = m_status.findItem();
		boost::optional<Enemy> enemyTarget = m_status.findEnemy();
		bool underEnemy = m_status.isUnderEnemy();

		float_t bestScore = std::numeric_limits<float_t>::lowest();
		boost::optional<DIR> bestDir;
		boost::optional<bool> bestSlow;

		for (DIR dir : DIRS)
		{
			Path path(m_status, m_scene, itemTarget, enemyTarget, underEnemy);
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
			std::cout << "��·���ߡ�" << std::endl;
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
