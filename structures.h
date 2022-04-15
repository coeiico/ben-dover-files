#pragma once

struct Vec2 {
	float x, y;
};

struct Vec4 {
	float x, y, z, w;
};

struct Vec3 {
	float x, y, z;

	Vec3 operator +(Vec3 d) {
		return {
			x + d.x,
			y + d.y,
			z + d.z
		};
	}

	Vec3 operator -(Vec3 d) {
		return {
			x - d.x,
			y - d.y,
			z - d.z
		};
	}

	Vec3 operator *(float d) {
		return {
			x * d,
			y * d,
			z * d
		};
	}

	void Normalize() {
		while (y < -180) y += 360;
		while (y > 180) y -= 360;
		if (x > 89) x = 89;
		if (x < -89) x = -89;
	}

	float Sum() {
		return x + y + z;
	}
};

struct Rect {
	Vec2 top_left;
	Vec2 top_right;
	Vec2 bottom_left;
	Vec2 bottom_right;
};

struct HyperRect {


	//    .b------c
	//  .' |    .'|
	// a---+--d'  |
	// |   |  |   |
	// |  ,+--+---+
	// |.'    | .'
	// +------+'

	struct {
		Vec2 a;		// Position on 2d screen
		float ax;	// x, y and z values at that 2d position.
		float ay;
		float az;
		Vec2 b;
		float bx;
		float by;
		float bz;
		Vec2 c;
		float cx;
		float cy;
		float cz;
		Vec2 d;
		float dx;
		float dy;
		float dz;
	} top;


	//    .+------+
	//  .' |    .'|
	// +---+--+'  |
	// |   |  |   |
	// |  ,b--+---c
	// |.'    | .'
	// a------d'

	struct {
		Vec2 a;		// Position on 2d screen
		float ax;	// x, y and z values at that 2d position.
		float ay;
		float az;
		Vec2 b;
		float bx;
		float by;
		float bz;
		Vec2 c;
		float cx;
		float cy;
		float cz;
		Vec2 d;
		float dx;
		float dy;
		float dz;
	} bottom;
};