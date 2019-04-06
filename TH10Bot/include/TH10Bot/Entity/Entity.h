#pragma once

namespace th
{
	// 实体结构
	// +-------+
	// | (x,y) |
	// |   +   | h
	// |       |
	// +-------+
	//     w
	class Entity
	{
	public:
		//Entity() :
		//	x(), y(), dx(), dy(), width(), height() {}
		//Entity(float_t x0, float_t y0, float_t dx0, float_t dy0, float_t width0, float_t height0) :
		//	x(x0), y(y0), dx(dx0), dy(dy0), width(width0), height(height0) {}

		float_t getDistance(const Pointf& other) const;
		float_t getDistance(const Entity& other) const;
		float_t getAngle(const Pointf& other) const;
		float_t getAngle(const Entity& other) const;
		FootPoint getFootPoint(const Pointf& other) const;
		FootPoint getFootPoint(const Entity& other) const;
		Direction getDirection() const;

		bool collide(const Entity& other) const;
		bool collide(const Entity& other, float_t frame) const;
		float_t willCollideWith(const Entity& other) const;
		float_t willCollideWith(const Entity& other, float_t frame) const;

		Pointf getPosition() const;
		void setPosition(const Pointf& pos);
		Pointf getTopLeft() const;
		Pointf getTopRight() const;
		Pointf getBottomLeft() const;
		Pointf getBottomRight() const;
		bool isHolding() const;
		Pointf getDelta() const;
		Pointf getNextPos() const;
		Sizef getSize() const;
		Rectf getRect() const;

		float_t x;	// 坐标在中心点
		float_t y;
		float_t dx;
		float_t dy;
		float_t width;
		float_t height;
	};
}
