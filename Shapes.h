#pragma once

#include "basic.h"

class Shape
{
public:
	Shape() {}
	~Shape() {}
	// if intersect with ray
	virtual bool Intersect(const Point &p, const Vector2 &d) = 0;
	// if intersect with ray, and compute a ray-circle intersection
	virtual bool Intersect(const Point &p, const Vector2 &d, Point& inter) = 0;
	// if the point inside the shape
	virtual bool IsInside(const Point &p) = 0;
	// get the normal of the point
	virtual Vector2 GetNormal(const Point &p) = 0;
};

class Circle :public Shape
{
protected:
	Point center;	// position of the circle
	float radius;	// circle radius
public:
	Circle(Point o, float r) :center(o), radius(r) {}
	Point GetCenter() { return center; }
	float GetRadius() { return radius; }
	// if the point inside the circle
	virtual bool IsInside(const Point &p)
	{
		return (center - p).length() <= radius;
	}
	// get the normal of the point
	virtual Vector2 GetNormal(const Point &p)
	{
		return (p - center).normalize();
	}
	// if the ray intersect with the circle
	virtual bool Intersect(const Point &p, const Vector2 &d)
	{
		if (IsInside(p)) return true;			//if the point inside the circle, then intersect
		float proj = (center - p)*d;			//projection distance of vector po on ray
		if (proj <= 0) return false;			//direction of reflection
		Point foot = p + d * proj;				//foot position
		float dis = (center - foot).length();	//the distance between foot and circle center
		return dis < radius;
	}
	// Compute a ray-circle intersection
	virtual bool Intersect(const Point &p, const Vector2 &d, Point &inter)
	{
		if (IsInside(p))
		{
			float proj = (center - p)*d;
			Point foot = p + d*proj;
			float dis1 = (center - foot).length();
			float dis2 = sqrt(radius*radius - dis1*dis1);
			inter = foot + d*dis2;
			return true;
		}
		float proj = (center - p)*d;				//projection distance of vector po on ray
		if (proj <= 0) 
			return false;							//no intersection
		Point foot = p + d * proj;					//foot position
		float dis1 = (center - foot).length();		//the distance between foot and circle center
		if (dis1 > radius)return false;
		float dis2 = sqrt(radius*radius - dis1*dis1);//distance between foot and intersection
		inter = foot - d*dis2;						//position of intersection
		return true;
	}
};