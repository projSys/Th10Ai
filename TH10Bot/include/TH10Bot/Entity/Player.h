#pragma once

#include "TH10Bot/Entity/Entity.h"

namespace th
{
	class Player :
		public Entity
	{
	public:
		//Player() :
		//	powers(), life(), type(), slow(), itemObtainRange(), status(), invinibleTime() {}
		//Player(float_t x0, float_t y0, float_t dx0, float_t dy0, float_t width0, float_t height0) :
		//	Entity(x0, y0, dx0, dy0, width0, height0),
		//	powers(), life(), type(), slow(), itemObtainRange(), status(), invinibleTime() {}

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
