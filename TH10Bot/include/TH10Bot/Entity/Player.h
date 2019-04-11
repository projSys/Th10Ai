#pragma once

#include "TH10Bot/Entity/Entity.h"

namespace th
{
	struct Player :
		Entity
	{
		Player advance(float_t frame) const;

		float_t powers;
		int_t life;
		// type == 0 Reimu
		// type == 1 Marisa
		int_t type;
		// slow == 0 High Speed
		// slow == 1 Low Speed
		int_t slow;
		float_t itemObtainRange;
		// status == 0 ����״̬����ʱ�޵�
		// status == 1 ����״̬
		// status == 2 ����
		// status == 3 Unknown
		// status == 4 ��������������ʱ��B���Ծ���
		int_t status;
		// ʣ���޵�ʱ��
		int_t invinibleTime;
	};
}
