#pragma once

#include "TH10Bot/Entity.h"

namespace th
{
	class Item :
		public Entity
	{
	public:
		Item advance(const Pointf& pos) const;
		Item advance(float_t frame) const;

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
		int_t type;
	};
}
