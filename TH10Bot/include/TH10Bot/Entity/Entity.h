#pragma once

namespace th
{
	// ʵ��ṹ
	// +-------+
	// | (x,y) |
	// |   +   | h
	// |       |
	// +-------+
	//     w
	struct Entity
	{
		float_t getDist(const Entity& other) const;
		float_t getAngle(const Entity& other) const;
		FootPoint getFootPoint(const Entity& other) const;
		Direction getDir() const;

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
