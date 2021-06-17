#pragma once

#include "Th10Ai/Common.h"

#include "Th10Ai/Entity/Entity.h"
#include "Th10Ai/Th10Raw.h"

namespace th
{
	class Item :
		public Entity
	{
	public:
		Item();
		Item(const ItemRaw* raw, uint_t id);

	private:
		// ���״̬��Ϊ���¼���
		// status == 0 ��Ч�ĵ�
		// status == 1 �����Ŀ��յ�
		// status == 2 ��B�����ӵ������ĵ�
		// status == 3 �����յ��ߡ���B���Զ����յĵ�
		// status == 4 ��������ȡ��Χ���Զ����յĵ�
		int_t m_status;
		// �������Ϊ���¼���
		// type == 1 Power Items P�㣨��㣩
		// type == 2 Point Items �÷ֵ㣨���㣩
		// type == 3 Faith Items �����㣨�̵㣩
		// type == 4 Large Power Items ��P�㣨��㣩
		// type == 5 Large Point Items ��÷ֵ㣨����ɫ���ߵ����㣩����BOSS����
		// type == 6 Unknown
		// type == 7 Life Items �����㣨�ϵ㡢1UP�㣩
		// type == 8 Unknown
		// type == 9 Faith Items �����㣨�̵㣩��������ʱ��P��ת������
		// type == 10 Power Items P�㣨��㣩����BOSS����
		int_t m_type;
		uint_t m_id;
	};
}
