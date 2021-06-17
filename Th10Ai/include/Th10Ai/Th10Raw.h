#pragma once

#include "Th10Ai/Common.h"

namespace th
{
	constexpr uint_t ITEM_MAX_COUNT = 2198;

	constexpr uint_t BULLET_MAX_COUNT = 2001;

	struct PlayerRaw
	{
		byte_t unknown0[0x3C0];
		float32_t x;				// 0x3C0
		float32_t y;				// 0x3C4
		byte_t unknown1[0x28];
		int32_t dx;					// 0x3F0
		int32_t dy;					// 0x3F4
		byte_t unknown2[0x24];
		float32_t width;			// 0x41C
		float32_t height;			// 0x420
		byte_t unknown3[0x34];
		int32_t status;				// 0x458
		byte_t unknown4[0x3EB4];
		int32_t invincibleTime;		// 0x4310
		byte_t unknown5[0x160];
		int32_t slow;				// 0x4474
	};

	static_assert(sizeof(PlayerRaw) == 0x4478, "Size is not correct.");

	struct ItemRaw
	{
		byte_t unknow0[0x3AC];
		float32_t x;				// 0x3AC
		float32_t y;				// 0x3B0
		byte_t unknown1[0x4];
		float32_t dx;				// 0x3B8
		float32_t dy;				// 0x3BC
		byte_t unknown2[0x1C];
		int32_t status;				// 0x3DC
		int32_t type;				// 0x3E0
		byte_t unknown3[0xC];
	};

	static_assert(sizeof(ItemRaw) == 0x3F0, "Size is not correct.");

	struct ItemContainer
	{
		byte_t unknow0[0x14];
		ItemRaw items[ITEM_MAX_COUNT];	// 0x21CEA0
		uint32_t itemsCount;
		byte_t unknow1[0x8];
	};

	static_assert(sizeof(ItemContainer) == 0x21CEC0, "Size is not correct.");

	struct BulletRaw
	{
		byte_t unknow0[0x3B4];
		float32_t x;				// 0x3B4
		float32_t y;				// 0x3B8
		byte_t unknown1[0x4];
		float32_t dx;				// 0x3C0
		float32_t dy;				// 0x3C4
		byte_t unknown2[0x28];
		float32_t width;			// 0x3F0
		float32_t height;			// 0x3F4
		byte_t unknown3[0x4C];
		int16_t unknown4;
		int16_t status;				// 0x446
		byte_t unknown5[0x18];
		int32_t type;				// 0x460
		byte_t unknown6[0x38C];
	};

	static_assert(sizeof(BulletRaw) == 0x7F0, "Size is not correct.");

	struct BulletContainer
	{
		byte_t unknow0[0x5C];
		uint32_t bulletsCount;
		BulletRaw bullets[BULLET_MAX_COUNT];	// 0x3E0AF0
		byte_t unknow1[0x4];
	};

	static_assert(sizeof(BulletContainer) == 0x3E0B54, "Size is not correct.");
}
