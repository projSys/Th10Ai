#pragma once

#include "Th10Hook/Common.h"

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

#include "Th10Hook/Console.h"
#include "Th10Hook/Status.h"
#include "Th10Hook/Scene.h"
#include "Th10Hook/Input.h"

#include <vector>

namespace th
{
	struct Node
	{
		Node() :
			valid(false),
			dir(DIR::HOLD),
			slow(false),
			score(_F(0.0)),
			parent(nullptr)
		{
		}

		// 选择：逐层获取最高分的节点
		Node* select()
		{
			Node* highestNode = this;
			while (true)
			{
				if (highestNode->isLeaf())
					break;

				float_t highestScore = std::numeric_limits<float_t>::lowest();
				Node* highestChild = nullptr;
				for (Node& child : highestNode->children)
				{
					if (child.valid && child.score > highestScore)
					{
						highestScore = child.score;
						highestChild = &child;
					}
				}
				// 如果子节点都无效，父节点也是无效的，无效节点不会被选中
				highestNode = highestChild;
			}
			return highestNode;
		}

		// 扩展：扩展子节点，检测是否可行
		void expand(const Player& player, bool slowFirst)
		{
			children.resize(to_underlying(DIR::MAX_COUNT));
			for (DIR dir : DIRS)
			{
				//Player childPlayer = player;
				//childPlayer.pos = pos;
				//childPlayer.advance(action.fromDir, slowFirst);

				//child.parent = this;
			}
		}

		// 模拟：计算节点得分
		void simulate()
		{

		}

		// 反向传播：更新到根节点路径上的节点数据
		void backPropagate()
		{

		}

		bool isRoot() const
		{
			return parent == nullptr;
		}

		bool isLeaf() const
		{
			return children.empty();
		}

		vec2 pos;
		int_t frame;

		bool valid;
		DIR dir;
		bool slow;
		float_t score;

		Node* parent;
		std::vector<Node> children;
	};

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

		Input m_input;
		std::atomic<bool> m_inputUpdated;

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
