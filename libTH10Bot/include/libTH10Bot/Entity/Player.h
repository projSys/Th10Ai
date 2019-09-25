#pragma once

#include "libTH10Bot/Entity/Entity.h"

namespace th
{
	class Player :
		public Entity
	{
	public:
		static const Pointf INIT_POS;

		void advance(Direction dir, bool slow);

		bool isRebirthStatus() const;
		bool isNormalStatus() const;
		bool isColliding() const;

		static const Pointf MOVE_SPEED_MARISA[DIR_MAXCOUNT];
		static const Pointf MOVE_SPEED_HIGH[DIR_MAXCOUNT];
		static const Pointf MOVE_SPEED_SLOW[DIR_MAXCOUNT];

		// status == 0 ����״̬����ʱ�޵�
		// status == 1 ����״̬
		// status == 2 ����
		// status == 3 Unknown
		// status == 4 ��������������ʱ��B���Ծ���
		int_t status;
		// ʣ���޵�ʱ��
		int_t invinibleTime;
		// slow == 0 High Speed
		// slow == 1 Low Speed
		int_t slow;
		float_t powers;
		// type == 0 Reimu
		// type == 1 Marisa
		//int_t type;
		int_t life;
		float_t itemObtainRange;
	};
}
