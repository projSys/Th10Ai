#include "Th10Ai/Th10Ai.h"

#include <Base/Catcher.h>

#include "Th10Ai/Path.h"

#include <fstream>
#include <boost/program_options.hpp>
#include <Base/Logger.h>
#include <Base/ScopeGuard.h>
#include <Base/Error.h>
#include <Base/Windows/Apis.h>

#include "Th10Ai/DllInject.h"

namespace th
{
	namespace po = boost::program_options;

	Th10Ai::Th10Ai() :
		m_controlDone(false),
		m_handleDone(false),
		m_bombTime(0),
		m_bombCount(0),
		m_findItemTime(0),
		inputFrame(0),
		statusFrame(0),
		handleFrame(0)
	{
		m_sharedMemory = SharedMemory(ip::create_only, "Th10SharedMemory", 64 * 1024 * 1024);
		m_sharedData = m_sharedMemory.construct<SharedData>("Th10SharedData")(m_sharedMemory);
		if (m_sharedData == nullptr)
			throw Exception("Th10SharedData名称已被使用。");

		fs::path dir = Apis::GetModuleDir();
		fs::path logPath = dir / L"Th10Ai.log";
		g_logger.addFileLog(logPath);
		g_logger.addCommonAttributes();

		fs::path confPath = dir / L"Th10Ai.conf";
		std::ifstream ifs(confPath.c_str());
		po::options_description desc("Config");
		desc.add_options()
			("exe-path", po::value<std::string>(), "exe path")
			("dll-name", po::value<std::string>(), "dll name")
			("dump", po::value<bool>(), "dump");
		po::variables_map vm;
		po::store(po::parse_config_file(ifs, desc), vm);
		po::notify(vm);

		fs::path exePath = Apis::AnsiToWide(vm["exe-path"].as<std::string>());
		std::wstring dllName = Apis::AnsiToWide(vm["dll-name"].as<std::string>());
		fs::path exeDir = exePath.parent_path();

		{
			STARTUPINFOW si = {};
			si.cb = sizeof(si);
			PROCESS_INFORMATION pi = {};
			if (!CreateProcessW(exePath.c_str(), nullptr, nullptr, nullptr, FALSE, CREATE_SUSPENDED, nullptr, exeDir.c_str(), &si, &pi))
				throw Error(GetLastError());
			ON_SCOPE_EXIT([&pi]()
				{
					CloseHandle(pi.hThread);
			CloseHandle(pi.hProcess);
				});

			fs::path dllPath = dir / dllName;
			//DllInject::EnableDebugPrivilege();
			DllInject::Inject(pi.hProcess, dllPath);

			DWORD count = ResumeThread(pi.hThread);
			if (count == (DWORD)-1)
				throw Error(GetLastError());
		}

		bool dump = vm.count("dump") ? vm["dump"].as<bool>() : false;
		if (dump)
		{
			fs::path pdPath = dir / L"procdump.exe";
			std::wostringstream oss;
			oss << pdPath.c_str() << L" -accepteula -h -e -w " << exePath.filename().c_str();
			//oss << pdPath.c_str() << L" -accepteula -h -e " << pi.dwProcessId;
			std::wstring cmd = oss.str();

			STARTUPINFOW si = {};
			si.cb = sizeof(si);
			PROCESS_INFORMATION pi = {};
			if (!CreateProcessW(nullptr, const_cast<LPWSTR>(cmd.c_str()), nullptr, nullptr, FALSE, CREATE_NO_WINDOW, nullptr, dir.c_str(), &si, &pi))
				throw Error(GetLastError());
			ON_SCOPE_EXIT([&pi]()
				{
					CloseHandle(pi.hThread);
			CloseHandle(pi.hProcess);
				});
		}

		if (!m_sharedData->waitInit())
			throw Exception("Th10Hook初始化超时。");

		m_controlThread = std::thread(&Th10Ai::controlProc, this);
	}

	Th10Ai::~Th10Ai()
	{
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
		try
		{
			std::cout << "保持窗口焦点在风神录上，开始游戏，按A启动AI，按S停止AI。" << std::endl;
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
					std::this_thread::sleep_for(Time(16));
				}
			}
		}
		catch (...)
		{
			BASE_LOG(error) << Catcher() << std::endl;
			ExitProcess(1);
		}
	}

	void Th10Ai::start()
	{
		if (!m_sharedData->isActive())
		{
			m_sharedData->setActive(true);
			std::cout << "AI启动。" << std::endl;
		}
	}

	void Th10Ai::stop()
	{
		if (m_sharedData->isActive())
		{
			m_sharedData->setActive(false);
			std::cout << "AI停止。" << std::endl;
			std::cout << "决死：" << m_bombCount << std::endl;
		}
	}

	void Th10Ai::run()
	{
		while (!m_handleDone)
			handle();
	}

	bool Th10Ai::handle()
	{
		if (!m_sharedData->isActive())
		{
			std::this_thread::sleep_for(Time(16));
			return false;
		}

		if (!m_sharedData->waitUpdate())
		{
			std::cout << "退出。" << std::endl;
			m_handleDone = true;
			return false;
		}

		++handleFrame;
		//m_readableStatus->inputFrame = inputFrame;
		//m_readableStatus->statusFrame = statusFrame;
		//m_readableStatus->handleFrame = handleFrame;

		Time t1 = Clock::Now();

		m_status.copy(m_sharedData->getReadableStatus());
		m_status.updateExtra();

		//m_status2.copy(m_status1);
		//m_status1.copy(m_status0);
		//m_status0.copy(*m_readableStatus);

		//m_scene2.clearAll();
		//m_scene2.splitEnemies(m_status2.getEnemies());
		//m_scene2.splitBullets(m_status2.getBullets());
		//m_scene2.splitLasers(m_status2.getLasers());

		//m_scene1.clearAll();
		//m_scene1.splitEnemies(m_status1.getEnemies());
		//m_scene1.splitBullets(m_status1.getBullets());
		//m_scene1.splitLasers(m_status1.getLasers());

		Time t2 = Clock::Now();

		m_scene.clearAll();
		m_scene.splitEnemies(m_status.getEnemies());
		m_scene.splitBullets(m_status.getBullets());
		m_scene.splitLasers(m_status.getLasers());

		Time t3 = Clock::Now();

		Input& input = m_sharedData->getWritableInput();
		input.clear();

		handleBomb();
		handleTalk();
		handleShoot();
		handleMove();

		Time t4 = Clock::Now();
		if (t4 - t1 > Time(5))
			std::cout << t2 - t1 << ' ' << t3 - t2 << ' ' << t4 - t3 << std::endl;

		m_sharedData->notifyInput();

		return true;
	}

	// 处理炸弹
	bool Th10Ai::handleBomb()
	{
		if (m_status.getPlayer().isColliding())
		{
			Time now = Clock::Now();
			if (now - m_bombTime > Time(1000))
			{
				m_bombTime = now;

				//const Player& player = m_readableStatus->getPlayer();
				//std::optional<Bullet> target = m_readableStatus->collide(player, 0);
				//m_status1.collide(player, 1);
				////m_status2.collide(player, 2);
				//if (target.has_value())
				//{
				//	m_status1.collide(player, 1, target.value().id);
				//	//m_status2.collide(player, 2, target.value().id);

				//	RegionCollideResult rcr = m_scene.collideAll(player, 0, target.value());
				//	std::cout << rcr.collided << std::endl;
				//	RegionCollideResult rcr1 = m_scene1.collideAll(player, 1, target.value());
				//	std::cout << rcr1.collided << std::endl;
				//	//RegionCollideResult rcr2 = m_scene2.collideAll(player, 2, target.value());
				//	//std::cout << rcr2.collided << std::endl;
				//}

				Input& input = m_sharedData->getWritableInput();
				input.bomb();
				++m_bombCount;
				//std::cout << statusFrame - handleFrame << "/"
				//	<< handleFrame << "/"
				//	<< inputFrame - handleFrame << "/"
				//	<< m_readableStatus->getPlayer().stageFrame - handleFrame
				//	<< " 决死：" << m_bombCount << std::endl;
				return true;
			}
		}
		return false;
	}

	// 处理对话
	bool Th10Ai::handleTalk()
	{
		if (m_status.isTalking())
		{
			Input& input = m_sharedData->getWritableInput();
			input.skip();
			return true;
		}
		return false;
	}

	// 处理攻击
	bool Th10Ai::handleShoot()
	{
		if (m_status.haveEnemies())
		{
			Input& input = m_sharedData->getWritableInput();
			input.shoot();
			return true;
		}
		return false;
	}

	// 处理移动
	bool Th10Ai::handleMove()
	{
		if (!m_status.getPlayer().isNormalStatus())
			return false;

		std::optional<Item> itemTarget = findItem();
		std::optional<Enemy> enemyTarget = findEnemy();
		bool underEnemy = m_status.isUnderEnemy();
		bool slowFirst = (!itemTarget.has_value() && underEnemy);
		//bool slowFirst = false;

		float_t bestScore = std::numeric_limits<float_t>::lowest();
		std::optional<DIR> bestDir;
		std::optional<bool> bestSlow;

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
			Input& input = m_sharedData->getWritableInput();
			input.move(bestDir.value());
			if (bestSlow.value())
			{
				input.slow();
			}
		}
		else
		{
			std::cout << "无路可走。" << std::endl;
		}

		return true;
	}

	// 查找道具
	std::optional<Item> Th10Ai::findItem()
	{
		const Player& player = m_status.getPlayer();
		const std::vector<Item>& items = m_status.getItems();
		const std::vector<Enemy>& enemies = m_status.getEnemies();
		std::optional<Item> target;

		if (items.empty())
			return target;

		Time now = Clock::Now();
		// 查找冷却中
		if (now - m_findItemTime < Time(2000))
			return target;

		// 自机高于1/4屏
		if (player.pos.y < Scene::SIZE.y / 4)
		{
			// 进入冷却
			m_findItemTime = now;
			return target;
		}

		// 自机高于1/2屏，敌机多于5个
		if ((player.pos.y < Scene::SIZE.y / 2) && (enemies.size() > 5))
		{
			// 进入冷却
			m_findItemTime = now;
			return target;
		}

		float_t minDist = std::numeric_limits<float_t>::max();
		//float_t maxY = std::numeric_limits<float_t>::lowest();
		for (const Item& item : items)
		{
			// 道具在屏幕外
			if (!Scene::IsInScene(item.pos))
				continue;

			// 道具高于1/5屏
			if (item.pos.y < Scene::SIZE.y / 5)
				continue;

			// 道具不在自机1/4屏内
			float_t dy = std::abs(item.pos.y - player.pos.y);
			if (dy > Scene::SIZE.y / 4)
				continue;

			// 道具太靠近敌机
			bool tooClose = false;
			for (const Enemy& enemy : enemies)
			{
				if (item.distance(enemy) < 150)
				{
					tooClose = true;
					break;
				}
			}
			if (tooClose)
				continue;

			// 道具与自机距离最近
			float_t dist = item.distance(player);
			if (dist < minDist)
			{
				minDist = dist;
				target = item;
			}

			//if (item.y > maxY)
			//{
			//	maxY = item.y;
			//	target = item;
			//}
		}

		return target;
	}

	// 查找敌机
	std::optional<Enemy> Th10Ai::findEnemy()
	{
		const Player& player = m_status.getPlayer();
		const std::vector<Enemy>& enemies = m_status.getEnemies();
		std::optional<Enemy> target;

		if (enemies.empty())
			return target;

		// 自机高于1/4屏
		if (player.pos.y < Scene::SIZE.y / 4)
			return target;

		float_t minDist = std::numeric_limits<float_t>::max();
		for (const Enemy& enemy : enemies)
		{
			// 敌机在屏幕外
			if (!Scene::IsInScene(enemy.pos))
				continue;

			// 敌机在自机下面
			if (enemy.pos.y > player.pos.y)
				continue;

			// 敌机与自机X轴距离最近
			float_t dx = std::abs(enemy.pos.x - player.pos.x);
			if (dx < minDist)
			{
				minDist = dx;
				target = enemy;
			}
		}

		return target;
	}
}
