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
		// status == 0 无效
		// status == 1 正常状态
		// status == 2 8帧子弹初始状态
		// status == 3 放B、符卡结束时子弹消除状态
		// status == 5 最后一个空子弹的状态
		int_t m_status;
		// type == 0 28.0 大玉
		// type == 1 10.0 中玉
		// type == 2 8.0 椭弹 刀弹
		// type == 2 6.0         水光弹 ?????
		// type == 3 6.0 小玉 环玉
		// type == 3 4.0         札弹 ?????
		// type == 4 4.0 米弹 针弹 铳弹 鳞弹 链弹 杆菌弹
		// type == 5 4.0 点弹 菌弹
		int_t m_type;
		uint_t m_id;
	};
}
