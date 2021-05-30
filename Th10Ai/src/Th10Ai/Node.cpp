#include "Th10Ai/Node.h"

namespace th
{
	Node::Node() :
		m_parent(nullptr),
		m_dir(DIR::HOLD),
		m_frame(0),
		m_valid(false),
		m_slow(false),
		m_score(_F(0.0)),
		m_totalScore(_F(0.0)),
		m_avgScore(_F(0.0)),
		m_highestScore(_F(0.0)),
		m_childrenCount(0)
	{
	}

	Node* Node::select()
	{
		Node* highestNode = this;
		while (!highestNode->isLeaf())
		{
			float_t highestScore = std::numeric_limits<float_t>::lowest();
			Node* highestChild = nullptr;
			for (Node& child : highestNode->m_children)
			{
				if (!child.m_valid)
					continue;

				if (child.m_highestScore > highestScore)
				{
					highestScore = child.m_highestScore;
					highestChild = &child;
				}
			}
			if (highestChild == nullptr)
				BASE_THROW(Exception("当前节点的子节点都无效。"));
			// 如果子节点都无效，父节点也是无效的，无效节点不会被选中
			highestNode = highestChild;
		}
		return highestNode;
	}

	void Node::expand(Status& status, Scene& scene, const boost::optional<Item>& itemTarget,
		const boost::optional<Enemy>& enemyTarget, bool slowFirst)
	{
		m_childrenCount = to_underlying(DIR::MAX_COUNT);
		m_children.resize(to_underlying(DIR::MAX_COUNT));
		for (DIR dir : DIRS)
		{
			Node& child = m_children[to_underlying(dir)];
			child.m_parent = this;
			child.m_dir = dir;
			child.m_frame = m_frame + 1;

			child.simulate(status, scene, itemTarget, enemyTarget, slowFirst);
			child.backPropagate();
		}
	}

	void Node::simulate(Status& status, Scene& scene, const boost::optional<Item>& itemTarget,
		const boost::optional<Enemy>& enemyTarget, bool slowFirst)
	{
		// 检测节点是否有效
		Player player = status.getPlayer();
		player.setPosition(m_parent->m_pos);
		player.advance(m_dir, slowFirst);
		m_pos = player.pos;
		m_slow = slowFirst;

		RegionCollideResult rcr = {};
		if (!Scene::IsInPlayerRegion(player.pos)
			|| (rcr = scene.collideAll(player, m_frame)).collided)
		{
			player.setPosition(m_parent->m_pos);
			player.advance(m_dir, !slowFirst);
			m_pos = player.pos;
			m_slow = !slowFirst;

			if (!Scene::IsInPlayerRegion(player.pos)
				|| (rcr = scene.collideAll(player, m_frame)).collided)
			{
				return;
			}
		}

		m_valid = true;

		// 计算节点得分
		if (itemTarget.has_value())
		{
			m_score = Path::CalcNearScore(m_pos, itemTarget.value().pos) * _F(100.0);
		}
		else if (enemyTarget.has_value())
		{
			m_score = Path::CalcShootScore(m_pos, enemyTarget.value().pos) * _F(100.0);
		}
		else
		{
			m_score = Path::CalcNearScore(m_pos, Path::RESET_POS) * _F(100.0);
		}

		m_totalScore = m_parent->m_totalScore + m_score;
		m_avgScore = m_totalScore / m_frame;

		m_highestScore = m_score;
	}

	void Node::backPropagate()
	{
		Node* node = this;
		while (!node->isRoot())
		{
			if (!node->m_valid)
			{
				node->m_parent->m_childrenCount -= 1;
				if (node->m_parent->m_childrenCount == 0)
					node->m_parent->m_valid = false;
			}

			node->m_parent->m_avgScore = node->m_avgScore;

			if (node->m_highestScore > node->m_parent->m_highestScore)
				node->m_parent->m_highestScore = node->m_highestScore;

			node = node->m_parent;
		}
	}

	bool Node::isRoot() const
	{
		return m_parent == nullptr;
	}

	bool Node::isLeaf() const
	{
		return m_children.empty();
	}
}
