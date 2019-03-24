#pragma once

namespace th
{
	class Player;
	class Item;
	class Enemy;
	class Bullet;
	class Laser;

	struct FootPoint
	{
		float_t x;		// 垂足坐标
		float_t y;
		float_t frames;	// 到达垂足的帧数
	};

	// 实体结构
	// +-----------+
	// |           |
	// |   (x,y)   |
	// |     +     | h
	// |           |
	// |           |
	// +-----------+
	//       w

	class Entity
	{
	public:
		Entity() :
			x(), y(), dx(), dy(), width(), height() {}
		Entity(float_t x0, float_t y0, float_t dx0, float_t dy0, float_t width0, float_t height0) :
			x(x0), y(y0), dx(dx0), dy(dy0), width(width0), height(height0) {}

		float_t getDistance(const Pointf& other) const;
		float_t getDistance(const Entity& other) const;
		float_t getAngle(const Pointf& other) const;
		float_t getAngle(const Entity& other) const;
		FootPoint getFootPoint(const Pointf& other) const;
		FootPoint getFootPoint(const Entity& other) const;
		Direction getDirection() const;

		bool collide(const Entity& other) const;
		float_t willCollideWith(const Entity& other) const;

		Pointf getPosition() const;
		void setPosition(const Pointf& pos);
		Pointf getTopLeft() const;
		Pointf getTopRight() const;
		Pointf getBottomLeft() const;
		Pointf getBottomRight() const;
		bool isHolding() const;
		Pointf getNextPos() const;
		Sizef getSize() const;
		Rectf getRect() const;

		float_t x;		// 坐标在中心点
		float_t y;
		float_t dx;
		float_t dy;
		float_t width;
		float_t height;
	};

	class Player :
		public Entity
	{
	public:
		Player() :
			powers(), life(), type(), slow(), itemObtainRange(), status(), invinibleTime() {}
		Player(float_t x0, float_t y0, float_t dx0, float_t dy0, float_t width0, float_t height0) :
			Entity(x0, y0, dx0, dy0, width0, height0),
			powers(), life(), type(), slow(), itemObtainRange(), status(), invinibleTime() {}

		bool collideSAT(const Laser& laser) const;

		float_t powers;
		int_t life;
		// type == 0 Reimu
		// type == 1 Marisa
		int_t type;
		// slow == 0 High Speed
		// slow == 1 Low Speed
		int_t slow;
		float_t itemObtainRange;
		// status == 0 重生状态，此时无敌
		// status == 1 正常状态
		// status == 2 死亡
		// status == 3 Unknown
		// status == 4 被弹、决死，此时放B可以决死
		int_t status;
		// 剩余无敌时间
		int_t invinibleTime;
	};

	class Item :
		public Entity
	{
	public:
		Item() :
			type() {}
		Item(float_t x0, float_t y0, float_t dx0, float_t dy0, float_t width0, float_t height0) :
			Entity(x0, y0, dx0, dy0, width0, height0), type() {}

		int32_t type;
	};

	class Enemy :
		public Entity
	{
	public:
		Enemy() {};
		Enemy(float_t x0, float_t y0, float_t dx0, float_t dy0, float_t width0, float_t height0) :
			Entity(x0, y0, dx0, dy0, width0, height0) {}
	};

	class Bullet :
		public Entity
	{
	public:
		Bullet() {};
		Bullet(float_t x0, float_t y0, float_t dx0, float_t dy0, float_t width0, float_t height0) :
			Entity(x0, y0, dx0, dy0, width0, height0) {}
	};

	// 激光结构
	//     (x,y)
	// +-----+-----+
	// |     |     |
	// |     |     |
	// |     |     |
	// |     |     | h
	// |     |     |
	// |     |     |
	// |     |     |
	// +-----+-----+
	//       w

	class Laser :
		public Entity
	{
	public:
		Laser() :
			arc() {}
		Laser(float_t x0, float_t y0, float_t dx0, float_t dy0, float_t width0, float_t height0) :
			Entity(x0, y0, dx0, dy0, width0, height0), arc() {}

		Pointf getTopLeft() const;
		Pointf getTopRight() const;
		Pointf getBottomLeft() const;
		Pointf getBottomRight() const;

		float_t arc;
	};

	class SATBox
	{
	public:
		static bool Collide(float_t p1, float_t l1, float_t p2, float_t l2);

		Pointf topLeft;
		Pointf topRight;
		Pointf bottomLeft;
		Pointf bottomRight;
	};

	class LaserBox :
		public SATBox
	{
	public:
		LaserBox(const Laser& laser);

		bool collideSAT(const Player& player) const;
	};

	class PlayerBox :
		public SATBox
	{
	public:
		PlayerBox(const Player& player, const Laser& laser);

		bool collideSAT(const Laser& laser) const;
	};
}
