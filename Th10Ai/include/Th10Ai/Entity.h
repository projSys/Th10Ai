#pragma once

#include "Th10Ai/RawStatus.h"

namespace th
{
	struct FootPoint
	{
		float_t k;
		Pointf pos;
	};

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
		static float_t CalcDistance(const Pointf& A, const Pointf& B);
		// AB与AC的夹角
		static float_t CalcAngle(const Pointf& A, const Pointf& B, const Pointf& C);
		// 点C到线段AB的垂足
		static FootPoint CalcFootPoint(const Pointf& A, const Pointf& B, const Pointf& C);

		Entity();
		Entity(float_t x0, float_t y0, float_t dx0, float_t dy0, float_t width0, float_t height0);

		float_t calcDistance(const Pointf& pos) const;
		float_t calcAngle(const Pointf& pos) const;
		FootPoint calcFootPoint(const Pointf& pos) const;
		Direction calcDirection() const;
		Direction calcDirection(const Pointf& pos) const;

		void advance(float_t frame);
		bool collide(const Entity& other) const;
		bool collide(const Entity& other, float_t frame) const;
		pair<bool, float_t> willCollideWith(const Entity& other) const;
		bool isHighSpeedWith(const Entity& other) const;

		Pointf getPosition() const;
		void setPosition(const Pointf& pos);
		Pointf getLeftTop() const;
		Pointf getRightTop() const;
		Pointf getLeftBottom() const;
		Pointf getRightBottom() const;
		bool isHolding() const;
		Pointf getDelta() const;
		Pointf getNextPos() const;
		Sizef getSize() const;
		Rectf getRect() const;

		static const Direction SECTOR_TO_DIR[17];

		float_t x;			// 坐标在中心点
		float_t y;
		float_t dx;
		float_t dy;
		float_t width;
		float_t height;

		int_t id;
		int_t type;
	};
}
