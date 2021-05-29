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

		// ѡ������ȡ��߷ֵĽڵ�
		Node* select();

		// ��չ����չ�ӽڵ�
		void expand(Status& status, Scene& scene, const boost::optional<Item>& itemTarget,
			const boost::optional<Enemy>& enemyTarget, bool slowFirst);

		// ģ�⣺���ڵ��Ƿ���Ч������ڵ�÷�
		void simulate(Status& status, Scene& scene, const boost::optional<Item>& itemTarget,
			const boost::optional<Enemy>& enemyTarget, bool slowFirst);

		// ���򴫲������µ����ڵ�·���ϵĽڵ�����
		void backPropagate();

		bool isRoot() const;

		bool isLeaf() const;

	//private:
		// ��������
		Node* m_parent;
		DIR m_dir;
		int_t m_frame;

		// ����õ���
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
