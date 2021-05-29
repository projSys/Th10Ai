#pragma once

#include "Th10Ai/Common.h"

#include <vector>
#include <boost/optional.hpp>

#include "Th10Ai/Direction.h"
#include "Th10Ai/Status.h"
#include "Th10Ai/Scene.h"
#include "Th10Ai/Input.h"
#include "Th10Ai/Path.h"

namespace th
{
	class Node
	{
	public:
		Node();

		// 选择：逐层获取最高分的节点
		Node* select();

		// 扩展：扩展子节点
		void expand(Status& status, Scene& scene, const boost::optional<Item>& itemTarget,
			const boost::optional<Enemy>& enemyTarget, bool slowFirst);

		// 模拟：检测节点是否有效，计算节点得分
		void simulate(Status& status, Scene& scene, const boost::optional<Item>& itemTarget,
			const boost::optional<Enemy>& enemyTarget, bool slowFirst);

		// 反向传播：更新到根节点路径上的节点数据
		void backPropagate();

		bool isRoot() const;

		bool isLeaf() const;

	//private:
		// 传过来的
		Node* m_parent;
		DIR m_dir;
		int_t m_frame;

		// 计算得到的
		bool m_valid;
		vec2 m_pos;
		bool m_slow;
		float_t m_score;
		float_t m_totalScore;
		float_t m_avgScore;
		float_t m_highestScore;

		uint_t m_childrenCount;
		std::vector<Node> m_children;
	};
}
