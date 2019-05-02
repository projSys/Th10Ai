#pragma once

namespace th
{
	struct FootPoint
	{
		float_t x;		// 垂足坐标
		float_t y;
		float_t frame;	// 到达垂足的帧数
	};

	// 实体结构
	// +-------+
	// | (x,y) |
	// |   +   | h
	// |       |
	// +-------+
	//     w
	struct Entity
	{
		static float_t GetDist(const Pointf& A, const Pointf& B);
		static float_t GetAngle(const Pointf& A, const Pointf& B, const Pointf& C);

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

		float_t x;			// 坐标在中心点
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
