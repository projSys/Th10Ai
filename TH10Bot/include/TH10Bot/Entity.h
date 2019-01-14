#pragma once

#include <vector>

#include "TH10Bot/Point2.h"
#include "TH10Bot/Rect2.h"

namespace th
{
	struct Item;
	struct Enemy;
	struct Bullet;
	struct Laser;

	struct Player :
		Rect2d
	{
		Player() = default;
		Player(double x0, double y0, double w0, double h0) :
			Rect2d(x0 + 200.0, y0, w0, h0) {}

		bool hitTest(const Rect2d& other, double epsilon = 0.0) const;
		bool hitTestSAT(const Laser& laser, double epsilon = 0.0) const;

		bool hitTest(const std::vector<Enemy>& enemies, double epsilon = 0.0) const;
		bool hitTest(const std::vector<Bullet>& bullets, double epsilon = 0.0) const;
		bool hitTestSAT(const std::vector<Laser>& lasers, double epsilon = 0.0) const;

		float dx;
		float dy;

		float powers;

		int life;

		// type == 0 Reimu
		// type == 1 Marisa
		int type;

		// slow == 0 High Speed
		// slow == 1 Low Speed
		int slow;

		float itemObtainRange;

		// status == 0 重生状态，此时无敌
		// status == 1 正常状态
		// status == 2 死亡
		// status == 3 Unknown
		// status == 4 被弹、决死，此时放B可以决死
		int status;

		// 剩余无敌时间
		int invinibleTime;
	};

	struct Item :
		Rect2d
	{
		Item() = default;
		Item(double x0, double y0, double w0, double h0) :
			Rect2d(x0 + 200.0, y0, w0, h0) {}
	};

	struct Enemy :
		Rect2d
	{
		Enemy() = default;
		Enemy(double x0, double y0, double w0, double h0) :
			Rect2d(x0 + 200.0, y0, w0, h0) {}
	};

	struct Bullet :
		Rect2d
	{
		double dx;
		double dy;

		Bullet() :
			dx(0.0), dy(0.0) {}
		Bullet(double x0, double y0, double w0, double h0, double dx0, double dy0) :
			Rect2d(x0 + 200.0, y0, w0, h0), dx(dx0), dy(dy0) {}

		Bullet advance() const;
		double angle(const Player& player) const;
	};

	//       w
	// +---(x,y)---+
	// |     |     |
	// |     |     |
	// |     |     |
	// |     |     | h
	// |     |     |
	// |     |     |
	// |     |     |
	// +-----+-----+
	struct Laser :
		Rect2d
	{
		double arc;

		Laser() :
			arc(0.0) {}
		Laser(double x0, double y0, double w0, double h0, double arc0) :
			Rect2d(x0 + 200.0, y0, w0, h0), arc(arc0) {}
	};

	struct SATBox
	{
		double x1, y1;
		double x2, y2;
		double x3, y3;
		double x4, y4;

		//SATBox() :
		//	x1(0.0), y1(0.0),
		//	x2(0.0), y2(0.0),
		//	x3(0.0), y3(0.0),
		//	x4(0.0), y4(0.0) {}

	protected:
		static void PointRotate(double& x, double& y, double cx, double cy, double arc);
		static bool HitTest(double c1, double d1, double c2, double d2, double epsilon);
	};

	struct LaserBox :
		SATBox
	{
		//LaserBox() = default;
		LaserBox(const Laser& laser);

		bool hitTestSAT(const Player& player, double epsilon = 0.0) const;
	};

	struct PlayerBox :
		SATBox
	{
		//PlayerBox() = default;
		PlayerBox(const Player& player, const Laser& laser);

		bool hitTestSAT(const Laser& laser, double epsilon = 0.0) const;
	};
}
