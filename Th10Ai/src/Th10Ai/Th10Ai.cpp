#include "Th10Ai/Th10Ai.h"

#include <Base/Catcher.h>
#include <Base/Windows/ExceptFilter.h>
#include <Base/Windows/Apis.h>

#include "Th10Ai/Path.h"

#if RENDER
#ifdef _DEBUG
#pragma comment(lib, "opencv_world452d.lib")
#else
#pragma comment(lib, "opencv_world452.lib")
#endif
#endif

namespace th
{
	std::unique_ptr<Th10Ai> g_th10Ai;

	Th10Ai::Th10Ai(HWND window) :
		m_controlDone(false),
		m_handleDone(false),
		m_active(false),
		m_statusUpdated(false),
		m_bombTime(0),
		m_bombCount(0),
		m_findItemTime(0),
		m_inputUpdated(false),
		inputFrame(0),
		statusFrame(0),
		handleFrame(0)
#if RENDER
		, m_mat(cv::Size(640, 480), CV_8UC4)
#endif
	{
		ExceptFilter::SetThreadExceptionHandlers();

		SetForegroundWindow(window);

		RECT rect = {};
		GetWindowRect(window, &rect);
		int windowWidth = rect.right - rect.left;
		int windowHeight = rect.bottom - rect.top;
		int screenWidth = GetSystemMetrics(SM_CXSCREEN);
		int screenHeight = GetSystemMetrics(SM_CYSCREEN);
		int x = (screenWidth - windowWidth) / 2;
		int y = (screenHeight - windowHeight) / 2;
		SetWindowPos(window, nullptr, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

		char buf1[1024] = {};
		GetWindowTextA(window, buf1, 1023);
		std::wstring buf2 = Apis::MultiByteToWideChar(932, buf1);	// Shift-JIS
		std::string buf3 = Apis::WideToAnsi(buf2);
		SetWindowTextA(window, buf3.c_str());

		//ULONG_PTR icon = GetClassLongPtrA(window, GCLP_HICON);
		HICON icon = LoadIconA(GetModuleHandleA(nullptr), "IDI_ICON3");
		SendMessageA(window, WM_SETICON, ICON_SMALL, (LPARAM)icon);

		m_writableStatus = std::make_unique<Status>();
		m_intermediateStatus = std::make_unique<Status>();
		m_readableStatus = std::make_unique<Status>();

		m_writableInput = std::make_unique<Input>();
		m_intermediateInput = std::make_unique<Input>();
		m_readableInput = std::make_unique<Input>();

#if RENDER
		cv::namedWindow("Th10Ai");
#endif

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

#if RENDER
		cv::destroyWindow("Th10Ai");
#endif
	}

	bool IsKeyPressed(int vKey)
	{
		return (GetAsyncKeyState(vKey) & 0x8000) != 0;
	}

	void Th10Ai::controlProc()
	{
		ExceptFilter::SetThreadExceptionHandlers();
		try
		{
			std::cout << "Keep the focus on the game window, start the game, then press \'A\' to start AI, and press \'S\' to stop AI." << std::endl;
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
			BASE_LOG(error) << Catcher() << std::flush;
			throw;
		}
	}

	void Th10Ai::start()
	{
		if (!m_active)
		{
			m_active = true;
			std::cout << "AI start." << std::endl;
		}
	}

	void Th10Ai::stop()
	{
		if (m_active)
		{
			m_active = false;
			std::cout << "AI stop." << std::endl;
			std::cout << "DeathBomb: " << m_bombCount << std::endl;
		}
	}

	void Th10Ai::handleProc()
	{
		ExceptFilter::SetThreadExceptionHandlers();
		try
		{
			while (!m_handleDone)
			{
				if (m_active)
				{
					handle();
				}
				else
				{
					std::this_thread::sleep_for(Time(16));
				}
			}
		}
		catch (...)
		{
			BASE_LOG(error) << Catcher() << std::flush;
			throw;
		}
	}

	void Th10Ai::updateStatus()
	{
		if (!m_active)
			return;

		++statusFrame;

		m_writableStatus->clear();
		m_writableStatus->update();

		std::unique_lock<std::mutex> lock(m_statusMutex);
		if (m_statusUpdated)
			std::cout << "状态跳帧" << std::endl;
		m_writableStatus.swap(m_intermediateStatus);
		m_statusUpdated = true;
		m_statusCond.notify_one();
	}

	bool Th10Ai::handle()
	{
		{
			std::unique_lock<std::mutex> lock(m_statusMutex);
			if (!m_statusUpdated)
				m_statusCond.wait(lock);
			else
				std::cout << "Processing is too slow." << std::endl;
			m_readableStatus.swap(m_intermediateStatus);
			m_statusUpdated = false;
		}

		if (!m_active)
			return false;

		++handleFrame;
		m_readableStatus->inputFrame = inputFrame;
		m_readableStatus->statusFrame = statusFrame;
		m_readableStatus->handleFrame = handleFrame;

		Time t1 = Clock::Now();

		m_readableStatus->updateExtra();
#if RENDER
		//Time t1 = Clock::Now();

		cv::Scalar black(0, 0, 0);
		cv::Scalar white(255, 255, 255);
		cv::Scalar red(0, 0, 255);
		cv::Scalar green(0, 255, 0);
		cv::Scalar blue(255, 0, 0);

		m_mat = white;

		m_scene.clearAll();
		m_scene.splitEnemies(m_readableStatus->getEnemies());
		m_scene.splitBullets(m_readableStatus->getBullets());
		m_scene.splitLasers(m_readableStatus->getLasers());
		//m_scene.render(m_mat, m_readableStatus->getPlayer());

		{
			const Player& player = m_readableStatus->getPlayer();
			vec2 windowPos = Scene::ToWindowPos(player.m_topLeft);
			cv::Rect rect(int_t(windowPos.x), int_t(windowPos.y), int_t(player.m_size.x), int_t(player.m_size.y));
			cv::rectangle(m_mat, rect, black);
		}

		const std::vector<Item>& items = m_readableStatus->getItems();
		for (const Item& item : items)
		{
			vec2 windowPos = Scene::ToWindowPos(item.m_topLeft);
			cv::Rect rect(int_t(windowPos.x), int_t(windowPos.y), int_t(item.m_size.x), int_t(item.m_size.y));
			cv::rectangle(m_mat, rect, blue);
		}

		const std::vector<Enemy>& enemies = m_readableStatus->getEnemies();
		for (const Enemy& enemy : enemies)
		{
			vec2 windowPos = Scene::ToWindowPos(enemy.m_topLeft);
			cv::Rect rect(int_t(windowPos.x), int_t(windowPos.y), int_t(enemy.m_size.x), int_t(enemy.m_size.y));
			cv::rectangle(m_mat, rect, red);
		}

		const std::vector<Bullet>& bullets = m_readableStatus->getBullets();
		for (const Bullet& bullet : bullets)
		{
			vec2 windowPos = Scene::ToWindowPos(bullet.m_topLeft);
			cv::Rect rect(int_t(windowPos.x), int_t(windowPos.y), int_t(bullet.m_size.x), int_t(bullet.m_size.y));
			cv::rectangle(m_mat, rect, red);
		}

		const std::vector<Laser>& lasers = m_readableStatus->getLasers();
		for (const Laser& laser : lasers)
		{
			vec2 p1 = Scene::ToWindowPos(laser.m_topLeft);
			vec2 p2 = Scene::ToWindowPos(laser.m_topRight);
			vec2 p3 = Scene::ToWindowPos(laser.m_bottomRight);
			vec2 p4 = Scene::ToWindowPos(laser.m_bottomLeft);
			cv::line(m_mat, cv::Point(int_t(p1.x), int_t(p1.y)), cv::Point(int_t(p2.x), int_t(p2.y)), red);
			cv::line(m_mat, cv::Point(int_t(p2.x), int_t(p2.y)), cv::Point(int_t(p3.x), int_t(p3.y)), red);
			cv::line(m_mat, cv::Point(int_t(p3.x), int_t(p3.y)), cv::Point(int_t(p4.x), int_t(p4.y)), red);
			cv::line(m_mat, cv::Point(int_t(p4.x), int_t(p4.y)), cv::Point(int_t(p1.x), int_t(p1.y)), red);
		}

		cv::imshow("Th10Ai", m_mat);

		//Time t2 = Clock::Now();
		//std::cout << t2 - t1 << std::endl;
#else
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
		m_scene.splitEnemies(m_readableStatus->getEnemies());
		m_scene.splitBullets(m_readableStatus->getBullets());
		m_scene.splitLasers(m_readableStatus->getLasers());

		Time t3 = Clock::Now();

		m_writableInput->clear();

		handleBomb();
		handleTalk();
		handleShoot();
		handleMove();

		Time t4 = Clock::Now();
		if (t4 - t1 > Time(5))
			std::cout << t2 - t1 << ' ' << t3 - t2 << ' ' << t4 - t3 << std::endl;

		{
			std::unique_lock<std::mutex> lock(m_inputMutex);
			//if (m_inputUpdated)
			//{
			//	std::cout << "输入跳帧" << std::endl;
			//}
			m_writableInput.swap(m_intermediateInput);
			m_inputUpdated = true;
		}
#endif

		return true;
	}

	std::mutex g_recordMutex;
	bool g_recordUpdated;
	Record g_writableRecord = {};
	Record g_intermediateRecord = {};
	Record g_readableRecord = {};

	void Th10Ai::commitInput(DWORD size, LPVOID data)
	{
#if RENDER
		return;
#endif

		if (!m_active)
			return;

		++inputFrame;

		bool inputUpdated = false;
		{
			std::unique_lock<std::mutex> lock(m_inputMutex);
			if (m_inputUpdated)
			{
				m_readableInput.swap(m_intermediateInput);
				m_inputUpdated = false;
				inputUpdated = true;
			}
		}
		if (inputUpdated)
		{
			m_readableInput->commit(size, data);
		}
		else
		{
			bool recordUpdated = false;
			{
				std::unique_lock<std::mutex> lock(g_recordMutex);
				if (g_recordUpdated)
				{
					std::swap(g_readableRecord, g_intermediateRecord);
					g_recordUpdated = false;
					recordUpdated = true;
				}
			}
			if (recordUpdated)
			{
				Input input;
				input.move(g_readableRecord.dir);
				if (g_readableRecord.slow)
					input.slow();
				input.commit(size, data);
			}
			else
			{
				std::cout
					<< "1111111111111111111111 Input is too slow." << std::endl;
			}

			std::cout << statusFrame - handleFrame << "/"
				<< handleFrame << "/"
				<< inputFrame - handleFrame << "/"
				<< m_readableStatus->getPlayer().stageFrame - handleFrame
				<< " Input is too slow." << std::endl;
		}
	}

	// 处理炸弹
	bool Th10Ai::handleBomb()
	{
		if (m_readableStatus->getPlayer().isColliding())
		{
			Time now = Clock::Now();
			if (now - m_bombTime > Time(1000))
			{
				m_bombTime = now;

				//const Player& player = m_readableStatus->getPlayer();
				//boost::optional<Bullet> target = m_readableStatus->collide(player, 0);
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

				m_writableInput->bomb();
				++m_bombCount;
				std::cout << statusFrame - handleFrame << "/"
					<< handleFrame << "/"
					<< inputFrame - handleFrame << "/"
					<< m_readableStatus->getPlayer().stageFrame - handleFrame
					<< " DeathBomb: " << m_bombCount << std::endl;
				return true;
			}
		}
		return false;
	}

	// 处理对话
	bool Th10Ai::handleTalk()
	{
		if (m_readableStatus->isTalking())
		{
			m_writableInput->skip();
			return true;
		}
		return false;
	}

	// 处理攻击
	bool Th10Ai::handleShoot()
	{
		if (m_readableStatus->haveEnemies())
		{
			m_writableInput->shoot();
			return true;
		}
		return false;
	}

	// 处理移动
	bool Th10Ai::handleMove()
	{
		if (!m_readableStatus->getPlayer().isNormalStatus())
			return false;

		boost::optional<Item> itemTarget = findItem();
		boost::optional<Enemy> enemyTarget = findEnemy();
		bool underEnemy = m_readableStatus->isUnderEnemy();
		bool slowFirst = (!itemTarget.has_value() && underEnemy);
		//bool slowFirst = false;

#if 1
		float_t bestScore = std::numeric_limits<float_t>::lowest();
		boost::optional<DIR> bestDir;
		boost::optional<bool> bestSlow;

		for (DIR dir : DIRS)
		{
			Path path(*m_readableStatus, m_scene, itemTarget, enemyTarget, underEnemy);
			Result result = path.find(dir);

			if (result.valid && path.m_bestScore > bestScore)
			{
				bestScore = path.m_bestScore;
				bestDir = path.m_dir;
				bestSlow = result.slow;
				g_writableRecord = path.m_record[1];
			}
		}

		{
			std::unique_lock<std::mutex> lock(g_recordMutex);
			std::swap(g_writableRecord, g_intermediateRecord);
			g_recordUpdated = true;
		}

		if (bestDir.has_value() && bestSlow.has_value())
		{
			m_writableInput->move(bestDir.value());
			if (bestSlow.value())
				m_writableInput->slow();
		}
		else
		{
			std::cout << "No way to go." << std::endl;
		}
#else
		// 暂停MCTS，缺少历史数据辅助节点选择，退化成了BFS
		m_root = new Node();
		m_root->m_valid = true;
		m_root->m_pos = m_readableStatus->getPlayer().pos;

		int count = 0;
		while (true)
		{
			Node* highestLeaf = m_root->select();
			highestLeaf->expand(*m_readableStatus, m_scene, itemTarget, enemyTarget, slowFirst);
			if (highestLeaf == m_root && !highestLeaf->m_valid)
			{
				std::cout << "No way to go. 1" << std::endl;
				break;
			}

			++count;
			if (count >= 40)
				break;
		}

		float_t bestScore = std::numeric_limits<float_t>::lowest();
		boost::optional<DIR> bestDir;
		boost::optional<bool> bestSlow;
		for (Node& child : m_root->m_children)
		{
			if (child.m_valid && child.m_avgScore > bestScore)
			{
				bestScore = child.m_avgScore;
				bestDir = child.m_dir;
				bestSlow = child.m_slow;
			}
		}
		if (bestDir.has_value() && bestSlow.has_value())
		{
			m_writableInput->move(bestDir.value());
			if (bestSlow.value())
				m_writableInput->slow();
		}
		else
		{
			std::cout << "No way to go. 2" << std::endl;
		}

		delete m_root;
#endif

		return true;
	}

	// 查找道具
	boost::optional<Item> Th10Ai::findItem()
	{
		const Player& player = m_readableStatus->getPlayer();
		const std::vector<Item>& items = m_readableStatus->getItems();
		const std::vector<Enemy>& enemies = m_readableStatus->getEnemies();

		boost::optional<Item> target;

		if (items.empty())
			return target;

		Time now = Clock::Now();
		// 查找冷却中
		if (now - m_findItemTime < Time(2000))
			return target;

		// 自机高于1/4屏
		if (player.m_pos.y < Scene::SIZE.y / 4)
		{
			// 进入冷却
			m_findItemTime = now;
			return target;
		}

		// 自机高于1/2屏，敌机多于5个
		if ((player.m_pos.y < Scene::SIZE.y / 2) && (enemies.size() > 5))
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
			if (!Scene::IsInScene(item.m_pos))
				continue;

			// 道具高于1/5屏
			if (item.m_pos.y < Scene::SIZE.y / 5)
				continue;

			// 道具不在自机1/4屏内
			float_t dy = std::abs(item.m_pos.y - player.m_pos.y);
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
	boost::optional<Enemy> Th10Ai::findEnemy()
	{
		const Player& player = m_readableStatus->getPlayer();
		const std::vector<Enemy>& enemies = m_readableStatus->getEnemies();

		boost::optional<Enemy> target;

		if (enemies.empty())
			return target;

		// 自机高于1/4屏
		if (player.m_pos.y < Scene::SIZE.y / 4)
			return target;

		float_t minDist = std::numeric_limits<float_t>::max();
		for (const Enemy& enemy : enemies)
		{
			// 敌机在屏幕外
			if (!Scene::IsInScene(enemy.m_pos))
				continue;

			// 敌机在自机下面
			if (enemy.m_pos.y > player.m_pos.y)
				continue;

			// 敌机与自机X轴距离最近
			float_t dx = std::abs(enemy.m_pos.x - player.m_pos.x);
			if (dx < minDist)
			{
				minDist = dx;
				target = enemy;
			}
		}

		return target;
	}
}
