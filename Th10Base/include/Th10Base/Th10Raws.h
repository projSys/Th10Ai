#pragma once

#include "Th10Base/Common.h"

namespace th
{
	constexpr uint_t ITEM_MAX_COUNT = 2198;

	constexpr uint_t BULLET_MAX_COUNT = 2001;

#pragma pack(1)

	struct GlobalVar
	{
		float32_t power;
		// character == 0 Reimu
		// character == 1 Marisa
		int32_t character;
		int32_t player;
		float32_t itemGetRange;

		int32_t stageFrame;
	};

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

	static_assert(sizeof(PlayerRaw) == 0x4478);

	struct ItemRaw
	{
		byte_t unknown0[0x3AC];
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

	static_assert(sizeof(ItemRaw) == 0x3F0);

	struct ItemContainer
	{
		byte_t unknown0[0x14];
		ItemRaw items[ITEM_MAX_COUNT];	// 0x21CEA0
		uint32_t itemCount;
		byte_t unknown1[0x8];
	};

	static_assert(sizeof(ItemContainer) == 0x21CEC0);

	struct EnemyRaw;

	// 大小未知，在EnemyRaw内部，成员或基类？
	struct EnemyElement
	{
		EnemyRaw* raw;
		EnemyElement* next;
	};

	struct EnemyRaw
	{
		byte_t unknown0[0x1068];
		float32_t x;				// 0x1068
		float32_t y;				// 0x106C
		byte_t unknown1[0x4];
		float32_t dx;				// 0x1074
		float32_t dy;				// 0x1078
		byte_t unknown2[0x78];
		float32_t width;			// 0x10F4
		float32_t height;			// 0x10F8
		byte_t unknown3[0x70];
		EnemyElement element;		// 0x116C
		byte_t unknown4[0x130C];
		int32_t status;				// 0x2480
		byte_t unknown5[0x94];
	};

	static_assert(sizeof(EnemyRaw) == 0x2518);

	struct EnemyContainer
	{
		byte_t unknown0[0x58];
		EnemyElement* head;
		byte_t unknown1[0xC];
	};

	static_assert(sizeof(EnemyContainer) == 0x68);

	struct BulletRaw
	{
		byte_t unknown0[0x3B4];
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

	static_assert(sizeof(BulletRaw) == 0x7F0);

	struct BulletContainer
	{
		byte_t unknown0[0x5C];
		uint32_t bulletCount;
		BulletRaw bullets[BULLET_MAX_COUNT];	// 0x3E0AF0
		byte_t unknown1[0x4];
	};

	static_assert(sizeof(BulletContainer) == 0x3E0B54);

	// 有2种大小 0xD58 0xD74
	struct LaserRaw
	{
		byte_t unknown0[0x8];
		LaserRaw* next;				// 0x8
		byte_t unknown1[0x18];
		float32_t x;				// 0x24
		float32_t y;				// 0x28
		byte_t unknown2[0x4];
		float32_t dx;				// 0x30
		float32_t dy;				// 0x34
		byte_t unknown3[0x4];
		float32_t arc;				// 0x3C
		// 高度在前，宽度在后
		float32_t height;			// 0x40
		float32_t width;			// 0x44
		byte_t unknown4[0xD10];
	};

	static_assert(sizeof(LaserRaw) == 0xD58);

	// head 赋值地址 0041C588 sub_41C510
	struct LaserContainer
	{
		byte_t unknown0[0x18];
		LaserRaw* head;
		byte_t unknown1[0x440];
	};

	static_assert(sizeof(LaserContainer) == 0x45C);

#pragma pack()
}
