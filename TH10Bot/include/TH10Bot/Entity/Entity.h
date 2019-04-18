#pragma once

namespace th
{
	struct FootPoint
	{
		float_t x;		// ��������
		float_t y;
		float_t frame;	// ���ﴹ���֡��
	};

	// ��X��������ĽǶ�����ת�����ƶ�����
	const Direction SECTOR_TO_DIR[16] =
	{
		DIR_RIGHT,
		DIR_UPRIGHT,
		DIR_UPRIGHT,
		DIR_UP,
		DIR_UP,
		DIR_UPLEFT,
		DIR_UPLEFT,
		DIR_LEFT,
		DIR_LEFT,
		DIR_DOWNLEFT,
		DIR_DOWNLEFT,
		DIR_DOWN,
		DIR_DOWN,
		DIR_DOWNRIGHT,
		DIR_DOWNRIGHT,
		DIR_RIGHT
	};

	// ʵ��ṹ
	// +-------+
	// | (x,y) |
	// |   +   | h
	// |       |
	// +-------+
	//     w
	struct Entity
	{
		float_t getDist(const Pointf& pos) const;
		FootPoint getFootPoint(const Pointf& pos) const;
		float_t getAngle(const Pointf& pos) const;
		Direction getDir() const;
		Direction getDir(const Pointf& pos) const;

		Entity advance(float_t frame) const;
		bool collide(const Entity& other) const;
		float_t willCollideWith(const Entity& other) const;

		Pointf getPos() const;
		void setPos(const Pointf& pos);
		Pointf getTopLeft() const;
		Pointf getTopRight() const;
		Pointf getBottomLeft() const;
		Pointf getBottomRight() const;
		bool isHolded() const;
		Pointf getDelta() const;
		Pointf getNextPos() const;
		Sizef getSize() const;
		Rectf getRect() const;

		float_t x;			// ���������ĵ�
		float_t y;
		float_t dx;
		float_t dy;
		float_t width;
		float_t height;
	};

	struct EntityView
	{
		uint_t index;
		Direction dir;
	};
}
