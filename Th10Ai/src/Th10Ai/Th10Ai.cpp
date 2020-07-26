#include "Th10Ai/Common.h"
#include "Th10Ai/Th10Ai.h"

#include "Th10Ai/MyWindow.h"
#include "Th10Ai/Path.h"

namespace th
{
	Th10Ai::Th10Ai(MyWindow* window) :
		m_window(window),
		m_controlDone(false),
		m_controlActive(false),
		m_readDone(false),
		m_readActive(false),
		m_handleDone(false),
		m_handleActive(false),
		m_bombCount(0),
		m_prevDir(DIR_HOLD),
		m_prevSlow(false),
		m_dataUpdated(false),
		p(true)
	{
		m_writeData = make_shared<Data>();
		m_middleData = make_shared<Data>();
		m_readData = make_shared<Data>();

		m_scene.split(6);

		AllocConsole();
		freopen("conin$", "r", stdin);
		freopen("conout$", "w", stdout);
		freopen("conout$", "w", stderr);

		HWND console = GetConsoleWindow();
		HMENU menu = GetSystemMenu(console, FALSE);
		EnableMenuItem(menu, SC_CLOSE, MF_GRAYED | MF_BYCOMMAND);

		m_controlThread = thread(&Th10Ai::controlProc, this);
		m_readThread = thread(&Th10Ai::readProc, this);
		m_handleThread = thread(&Th10Ai::handleProc, this);
	}

	Th10Ai::~Th10Ai()
	{
		m_handleDone = true;
		if (m_handleThread.joinable())
			m_handleThread.join();

		m_readDone = true;
		//m_d3d9Hook.notifyPresent();
		m_context.notifyUpdate();
		if (m_readThread.joinable())
			m_readThread.join();

		m_controlDone = true;
		if (m_controlThread.joinable())
			m_controlThread.join();
	}

	bool Th10Ai::IsKeyPressed(int vKey)
	{
		return (GetAsyncKeyState(vKey) & 0x8000) != 0;
	}

	void Th10Ai::controlProc()
	{
		try
		{
			cout << "请将焦点放在风神录窗口上，开始游戏，然后按A启动AI，按S停止AI，按D退出AI。" << endl;

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
				else if (IsKeyPressed('D'))
				{
					break;
				}
				else if (IsKeyPressed('F'))
				{
					print();
				}

				this_thread::sleep_for(std::chrono::milliseconds(16));
			}

			stop();
			cout << "退出AI。" << endl;
		}
		catch (...)
		{
			BASE_LOG_ERROR(PrintException());
		}
	}

	void Th10Ai::start()
	{
		if (!m_controlActive)
		{
			m_controlActive = true;
			m_readActive = true;
			m_handleActive = true;

			//m_di8Hook.clear();
			//m_di8Hook.commit();
			//m_d3d9Hook.enable(true);
			//m_di8Hook.enable(true);

			cout << "启动AI。" << endl;
		}
	}

	void Th10Ai::stop()
	{
		if (m_controlActive)
		{
			m_controlActive = false;
			m_readActive = false;
			m_handleActive = false;

			//m_di8Hook.enable(false);
			//m_d3d9Hook.enable(false);
			//m_di8Hook.clear();
			//m_di8Hook.commit();

			cout << "停止AI。" << endl;
			cout << "决死总数：" << m_bombCount << endl;
		}
	}

	void Th10Ai::print()
	{
		//p = true;
		//m_data.print();
	}

	void Th10Ai::readProc()
	{
		try
		{
			while (!m_readDone)
			{
				if (m_readActive)
				{
					//if (!m_d3d9Hook.waitPresent())
					//	cout << "CPU太慢了，数据读取不及时。" << endl;

					//m_data.update();
					//m_data.getPlayer().checkPrevMove(m_prevDir, m_prevSlow);
					//m_writeData->update();
					m_context.waitUpdate();
					m_window->update(m_context.getStatus());
					//cout << m_context.getStatus().player.width << endl;

					lock_guard<mutex> lock(m_dataMutex);
					std::swap(m_writeData, m_middleData);
					m_dataUpdated = true;
				}
				else
				{
					this_thread::sleep_for(std::chrono::milliseconds(16));
				}
			}
		}
		catch (...)
		{
			BASE_LOG_ERROR(PrintException());
			m_controlDone = true;
		}
	}

	Status_t& Th10Ai::getStatus()
	{
		return m_context.getStatus();
	}

	void Th10Ai::handleProc()
	{
		try
		{
			while (!m_handleDone)
			{
				if (m_handleActive)
				{
					handle();
				}
				else
				{
					this_thread::sleep_for(std::chrono::milliseconds(16));
				}
			}
		}
		catch (...)
		{
			BASE_LOG_ERROR(PrintException());
			m_controlDone = true;
		}
	}

	void Th10Ai::handle()
	{
		//if (!m_d3d9Hook.waitPresent())
		//	cout << "读取不及时。" << endl;

		//static int_t fps = 0;
		//static std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
		//++fps;
		//std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
		//std::chrono::milliseconds interval = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
		//if (interval.count() >= 1000)
		//{
		//	cout << "fps: " << fps << endl;
		//	fps = 0;
		//	begin += std::chrono::milliseconds(1000);
		//}

		//static std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
		//std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
		//time_t interval = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
		//if (interval > 17)
		//	cout << "掉帧：" << interval << endl;
		//begin = end;

		//std::chrono::steady_clock::time_point t0 = std::chrono::steady_clock::now();

		//m_data.update();
		//m_data.getPlayer().checkPrevMove(m_prevDir, m_prevSlow);

		//std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();
		//time_t e1 = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
		//cout << "e1: " << e1 << endl;

		if (m_dataUpdated)
		{
			lock_guard<mutex> lock(m_dataMutex);
			std::swap(m_readData, m_middleData);
			m_dataUpdated = false;
		}
		else
		{
			return;
		}

		m_scene.clearAll();
		m_scene.splitEnemies(m_readData->getEnemies());
		m_scene.splitBullets(m_readData->getBullets());
		m_scene.splitLasers(m_readData->getLasers());

		//std::chrono::steady_clock::time_point t2 = std::chrono::steady_clock::now();
		//time_t e2 = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
		//cout << "e2: " << e2 << endl;

		//if (p)
		//{
		//	CellCollideResult ccResult = m_scene.collideAll(m_data.getPlayer(), 0.0f);
		//	cout << ccResult.collided;
		//}

		handleBomb();
		handleTalk();
		handleShoot();
		handleMove();

		//if (p)
		//{
		//	Player temp = m_data.getPlayer();
		//	temp.advance(m_prevDir, m_prevSlow);
		//	CellCollideResult ccResult1 = m_scene.collideAll(temp, 1.0f);
		//	cout << " " << ccResult1.collided << endl;
		//}

		//std::chrono::steady_clock::time_point t3 = std::chrono::steady_clock::now();
		//time_t e3 = std::chrono::duration_cast<std::chrono::milliseconds>(t3 - t2).count();
		//cout << "e3: " << e3 << endl;

		//m_di8Hook.commit();
	}

	// 处理炸弹
	bool Th10Ai::handleBomb()
	{
		if (m_readData->getPlayer().isColliding())
		{
			//p = false;
			//m_data.print();

			//CellCollideResult ccr = m_scene.collideAll(m_data.getPlayer(), 0.0f);
			//cout << ccr.collided << endl;

			//m_di8Hook.keyPress(DIK_X);
			++m_bombCount;
			cout << "决死：" << m_bombCount << endl;
			return true;
		}
		else
		{
			//m_di8Hook.keyRelease(DIK_X);
			return false;
		}
	}

	// 处理对话
	bool Th10Ai::handleTalk()
	{
		if (m_readData->isTalking())
		{
			//m_di8Hook.keyPress(DIK_LCONTROL);
			return true;
		}
		else
		{
			//m_di8Hook.keyRelease(DIK_LCONTROL);
			return false;
		}
	}

	// 处理攻击
	bool Th10Ai::handleShoot()
	{
		if (m_readData->hasEnemy())
		{
			//m_di8Hook.keyPress(DIK_Z);
			return true;
		}
		else
		{
			//m_di8Hook.keyRelease(DIK_Z);
			return false;
		}
	}

	// 处理移动
	bool Th10Ai::handleMove()
	{
		if (!m_readData->getPlayer().isNormalStatus())
			return false;

		ItemTarget itemTarget = m_readData->findItem();
		EnemyTarget enemyTarget = m_readData->findEnemy();
		bool underEnemy = m_readData->isUnderEnemy();

		float_t bestScore = numeric_limits<float_t>::lowest();
		Direction bestDir = DIR_NONE;
		bool bestSlow = false;

		for (Direction dir : DIRECTIONS)
		{
			Path path(*m_readData, m_scene, itemTarget, enemyTarget, underEnemy);
			Result result = path.find(dir);

			if (result.valid && path.m_bestScore > bestScore)
			{
				bestScore = path.m_bestScore;
				bestDir = path.m_dir;
				bestSlow = result.slow;
			}
		}

		if (bestDir != DIR_NONE)
		{
			move(bestDir, bestSlow);
		}
		else
		{
			move(DIR_HOLD, false);
			cout << "无路可走。" << endl;
		}

		return true;
	}

	void Th10Ai::move(Direction dir, bool slow)
	{
		//if (slow)
		//	m_di8Hook.keyPress(DIK_LSHIFT);
		//else
		//	m_di8Hook.keyRelease(DIK_LSHIFT);

		switch (dir)
		{
		case DIR_HOLD:
			//m_di8Hook.keyRelease(DIK_LEFT);
			//m_di8Hook.keyRelease(DIK_RIGHT);
			//m_di8Hook.keyRelease(DIK_UP);
			//m_di8Hook.keyRelease(DIK_DOWN);
			break;

		case DIR_LEFT:
			//m_di8Hook.keyPress(DIK_LEFT);
			//m_di8Hook.keyRelease(DIK_RIGHT);
			//m_di8Hook.keyRelease(DIK_UP);
			//m_di8Hook.keyRelease(DIK_DOWN);
			break;

		case DIR_RIGHT:
			//m_di8Hook.keyRelease(DIK_LEFT);
			//m_di8Hook.keyPress(DIK_RIGHT);
			//m_di8Hook.keyRelease(DIK_UP);
			//m_di8Hook.keyRelease(DIK_DOWN);
			break;

		case DIR_UP:
			//m_di8Hook.keyRelease(DIK_LEFT);
			//m_di8Hook.keyRelease(DIK_RIGHT);
			//m_di8Hook.keyPress(DIK_UP);
			//m_di8Hook.keyRelease(DIK_DOWN);
			break;

		case DIR_DOWN:
			//m_di8Hook.keyRelease(DIK_LEFT);
			//m_di8Hook.keyRelease(DIK_RIGHT);
			//m_di8Hook.keyRelease(DIK_UP);
			//m_di8Hook.keyPress(DIK_DOWN);
			break;

		case DIR_LEFTUP:
			//m_di8Hook.keyPress(DIK_LEFT);
			//m_di8Hook.keyRelease(DIK_RIGHT);
			//m_di8Hook.keyPress(DIK_UP);
			//m_di8Hook.keyRelease(DIK_DOWN);
			break;

		case DIR_RIGHTUP:
			//m_di8Hook.keyRelease(DIK_LEFT);
			//m_di8Hook.keyPress(DIK_RIGHT);
			//m_di8Hook.keyPress(DIK_UP);
			//m_di8Hook.keyRelease(DIK_DOWN);
			break;

		case DIR_LEFTDOWN:
			//m_di8Hook.keyPress(DIK_LEFT);
			//m_di8Hook.keyRelease(DIK_RIGHT);
			//m_di8Hook.keyRelease(DIK_UP);
			//m_di8Hook.keyPress(DIK_DOWN);
			break;

		case DIR_RIGHTDOWN:
			//m_di8Hook.keyRelease(DIK_LEFT);
			//m_di8Hook.keyPress(DIK_RIGHT);
			//m_di8Hook.keyRelease(DIK_UP);
			//m_di8Hook.keyPress(DIK_DOWN);
			break;
		}

		m_prevDir = dir;
		m_prevSlow = slow;
	}
}
