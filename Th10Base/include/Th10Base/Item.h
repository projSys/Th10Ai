#pragma once

#include "Th10Base/Common.h"

#include "Th10Base/Entity.h"
#include "Th10Base/Th10Raws.h"

namespace th
{
	class Item :
		public Entity
	{
	public:
		Item();
		Item(const ItemRaw* raw, uint_t id0);

	public:
		// 点的状态分为以下几种
		// status == 0 无效的点
		// status == 1 正常的可收点
		// status == 2 放B消除子弹产生的点
		// status == 3 到达收点线、放B等自动回收的点
		// status == 4 到达点的收取范围，自动回收的点
		int_t status;
		// 正常点分为以下几种
		// type == 1 Power Items P点（红点）
		// type == 2 Point Items 得分点（蓝点）
		// type == 3 Faith Items 信仰点（绿点）
		// type == 4 Large Power Items 大P点（红点）
		// type == 5 Large Point Items 大得分点（带黄色边线的蓝点），由BOSS掉落
		// type == 6 Unknown
		// type == 7 Life Items 续命点（紫点、1UP点）
		// type == 8 Unknown
		// type == 9 Faith Items 信仰点（绿点），满灵力时由P点转化而来
		// type == 10 Power Items P点（红点），由BOSS掉落
		int_t type;
		uint_t id;
	};
}
