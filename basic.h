#pragma once

#include <math.h>

#define EPSILON 1e-5f

struct Vector2 
{
	float x;
	float y;

	Vector2() : x(0), y(0) {}
	Vector2(float x, float y) : x(x), y(y) {}
	Vector2 operator+(const Vector2 &v) const	{ return{ x + v.x, y + v.y }; }
	Vector2 operator-(const Vector2 &v) const	{ return{ x - v.x, y - v.y}; }
	Vector2 operator-()	const					{ return{ -x, -y }; }
	// dot product
	float operator*(const Vector2 &v) const		{ return{ x*v.x + y*v.y }; }
	Vector2 operator*(const float &f) const		{ return{ x*f, y*f }; }
	Vector2 operator/(const float &f) const		{ return{ x / f, y / f }; }
	float length2() const						{ return{ x*x + y*y }; }
	float length() const						{ return sqrtf(length2()); }
	Vector2 reflect (const Vector2 normal) const
	{
		float idotn2 = (normal.x*x + normal.y*y)*-2.0f;
		return Vector2(x + idotn2*normal.x, y + idotn2*normal.y);
	}
	Vector2 normalize() const
	{
		float len = length2();
		if (len > 0)
		{
			float  invNor = 1 / sqrtf(len);
			return Vector2( x*invNor, y*invNor);
		}
		return Vector2(0.0f,0.0f);
	}
};

struct  Point
{
	float x;
	float y;

	Point() :x(0), y(0) {}
	Point(float x, float y) :x(x), y(y) {}
	Point operator+(const Vector2 &v) const { return{ x + v.x, y + v.y }; }
	Point operator+(const Point p)	  const { return{ x + p.x, y + p.y }; }
	Vector2 operator-(const Point &p) const { return Vector2( x - p.x, y - p.y ); }
	Point operator-(const Vector2 &v) const { return{ x - v.x, y - v.y }; }
	Point operator/(const float &f) const { return{ x/f, y/f }; }
	bool IsValid()	{ return(x >= 0.f && x <= 1.f && y > 0.f && y < 1.f); }
};

struct Color
{
	float r, g, b;

	Color() :r(0), g(0), b(0) {}
	Color(float r, float g, float b) :r(r), g(g), b(b) {}
	Color operator+(const Color &c) const { return{ r + c.r, g + c.g, b + c.b }; }
	Color operator*(const float &f) const { return{ r*f, g*f, b*f }; }
	Color operator*(const Color &c) const { return{ r*c.r, g*c.g, b*c.b }; }
	Color operator/(const float &f) const { return{ r / f, g / f, b / f }; }
	bool operator>(const Color &c) const { return{ r + g + b > c.r + c.g + c.b }; }
	bool operator<(const Color &c) const { return{ r + g + b < c.r + c.g + c.b }; }
};