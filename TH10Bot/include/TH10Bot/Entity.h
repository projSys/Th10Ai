#pragma once

namespace th
{
	class Player;
	class Item;
	class Enemy;
	class Bullet;
	class Laser;

	class Entity
	{
	public:
		Entity() :
			x(), y(), dx(), dy(), width(), height() {}
		Entity(float_t x0, float_t y0, float_t dx0, float_t dy0, float_t width0, float_t height0) :
			x(x0), y(y0), dx(dx0), dy(dy0), width(width0), height(height0) {}

		float_t distance(const Pointf& pos) const;
		float_t distance(const Entity& other) const;
		float_t angle(const Pointf& pos) const;
		float_t angle(const Entity& other) const;
		float_t footFrame(const Pointf& pos) const;
		Pointf footPoint(float_t footFrame) const;
		Direction direction() const;

		Pointf advanceTo(int_t frame) const;
		bool collide(const Entity& other, float_t epsilon) const;
		bool collide2(const Entity& other) const;

		Pointf getPos() const;
		void setPos(const Pointf& pos);
		Pointf getTopLeft() const;
		Pointf getBottomRight() const;
		bool isResting() const;
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

		bool collideSAT(const Laser& laser, float_t epsilon = 0.0f) const;

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
			radian() {}
		Laser(float_t x0, float_t y0, float_t dx0, float_t dy0, float_t width0, float_t height0) :
			Entity(x0, y0, dx0, dy0, width0, height0), radian() {}

		float_t radian;
	};

	class SATBox
	{
	public:
		static void PointRotate(float_t& x, float_t& y, float_t cx, float_t cy, float_t radian);
		static bool Collide(float_t c1, float_t d1, float_t c2, float_t d2, float_t epsilon);

		//SATBox() :
		//	x1(), y1(),
		//	x2(), y2(),
		//	x3(), y3(),
		//	x4(), y4() {}

		float_t x1, y1;
		float_t x2, y2;
		float_t x3, y3;
		float_t x4, y4;
	};

	struct LaserBox :
		SATBox
	{
		//LaserBox() = default;
		LaserBox(const Laser& laser);

		bool collideSAT(const Player& player, float_t epsilon = 0.0) const;
	};

	struct PlayerBox :
		SATBox
	{
		//PlayerBox() = default;
		PlayerBox(const Player& player, const Laser& laser);

		bool collideSAT(const Laser& laser, float_t epsilon = 0.0) const;
	};
}
