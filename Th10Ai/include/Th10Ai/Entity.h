#pragma once

#include "Th10Ai/Common.h"

#include "Th10Ai/Th10Data.h"
#include "Th10Ai/Direction.h"
#include "Th10Ai/Utils.h"

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
		static const DIR SECTOR_TO_DIR[17];

		Entity();
		Entity(const PlayerData& data);
		Entity(const ItemData& data);
		Entity(const EnemyData& data);
		Entity(const BulletData& data);
		Entity(const LaserData& data);
		Entity(float_t x0, float_t y0, float_t dx0, float_t dy0, float_t width0, float_t height0);

		float_t calcDistance(const Pointf& pos) const;
		float_t calcAngle(const Pointf& pos) const;
		FootPoint calcFootPoint(const Pointf& pos) const;
		DIR calcDirection() const;
		DIR calcDirection(const Pointf& pos) const;

		void advance(float_t frame);
		bool collide(const Entity& other) const;
		bool collide(const Entity& other, float_t frame) const;
		std::pair<bool, float_t> willCollideWith(const Entity& other) const;
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

		//int_t id;
		//int_t type;

		float_t x;
		float_t y;
		float_t dx;
		float_t dy;
		float_t width;
		float_t height;
	};
}
