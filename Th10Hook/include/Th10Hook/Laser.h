#pragma once

#include "Th10Hook/Common.h"

#include "Th10Hook/Entity.h"

namespace th
{
	// 激光结构
	// (x,y)
	// +-*-+
	// |   |
	// |   |
	// |   |
	// |   | h
	// |   |
	// |   |
	// |   |
	// +---+
	//   w
	// OBB
	class Laser
	{
	public:
		Laser();
		Laser(const vec2& pos0, const vec2& delta0, const vec2& size0, float_t arc0);

		void rotate();
		bool overlap(const vec2& lt2, const vec2& rt2, const vec2& lb2, const vec2& rb2,
			const vec2& axis) const;
		bool collide(const Entity& other) const;
		FootPoint calcFootPoint(const Entity& other) const;
		std::pair<bool, float_t> willCollideWith(const Entity& other) const;

		vec2 getCenter() const;
		vec2 getLeftTop() const;
		vec2 getRightTop() const;
		vec2 getLeftBottom() const;
		vec2 getRightBottom() const;
		vec2 getNextPos() const;
		void advance(float_t frame);

		vec2 pos;
		vec2 delta;
		vec2 size;
		float_t arc;

		// 旋转后的坐标
		vec2 leftTop;
		vec2 rightTop;
		vec2 leftBottom;
		vec2 rightBottom;
		vec2 axisX;	// 投影用的对称轴，单位向量
		vec2 axisY;
	};
}
