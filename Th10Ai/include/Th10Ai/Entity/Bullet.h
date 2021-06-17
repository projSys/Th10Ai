#pragma once

#include "Th10Ai/Common.h"

#include "Th10Ai/Entity/Entity.h"
#include "Th10Ai/Th10Raw.h"

namespace th
{
	class Bullet :
		public Entity
	{
	public:
		Bullet();
		Bullet(const BulletRaw* raw, uint_t id);

	//private:
		// status == 0 ��Ч
		// status == 1 ����״̬
		// status == 2 8֡�ӵ���ʼ״̬
		// status == 3 ��B����������ʱ�ӵ�����״̬
		// status == 5 ���һ�����ӵ���״̬
		int_t m_status;
		// type == 0 28.0 ����
		// type == 1 10.0 ����
		// type == 2 8.0 �ֵ� ����
		// type == 2 6.0         ˮ�ⵯ ?????
		// type == 3 6.0 С�� ����
		// type == 3 4.0         ���� ?????
		// type == 4 4.0 �׵� �뵯 掠� �۵� ���� �˾���
		// type == 5 4.0 �㵯 ����
		int_t m_type;
		uint_t m_id;
	};
}
