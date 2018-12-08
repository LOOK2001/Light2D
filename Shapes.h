#pragma once

#include "common.h"

class Shape
{
public:
	Shape() {}
	~Shape() {}
	// 判断是否相交
	virtual bool Intersect(const Point &p, const Vector2 &d) = 0;
	// 判断是否相交并求交点
	virtual bool Intersect(const Point &p, const Vector2 &d, Point& inter) = 0;
	// 判断是否在shape内部
	virtual bool IsInside(const Point &p) = 0;
	// 获取边界上的点所在位置的法线
	virtual Vector2 GetNormal(const Point &p) = 0;
	// 是否在边界上
	virtual bool IsOnBoundary(const Point &p) = 0;
	// 是否在包围盒内部
	virtual bool Contained(const float &left, const float &right, const float &up, const float &down) { return false; }
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
	bool IsInside(const Point &p)
	{
		return (center - p).length() <= radius;
	}
	Vector2 GetNormal(const Point &p)
	{
		return (p - center).normalize();
	}
	bool IsOnBoundary(const Point &p)
	{
		return fabs((center - p).length() - radius) <= EPSILON;
	}
	bool Intersect(const Point &p, const Vector2 &d)	//求p出发的d方向射线与Circle是否相交
	{
		if (IsInside(p)) return true;	//在圆内侧必然相交
		float proj = (center - p)*d;		//向量po在射线上垂足的距离
		if (proj <= 0) return false;	//反向射线
		Point foot = p + d * proj;		//垂足位置
		float dis = (center - foot).length(); //圆心到垂足的距离
		return dis < radius;
	}
	// Compute a ray-circle intersection
	bool Intersect(const Point &p, const Vector2 &d, Point &inter)
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
		float proj = (center - p)*d;					//向量po在射线上垂足的距离
		if (proj <= 0) return false;				//反向射线
		Point foot = p + d * proj;					//垂足位置
		float dis1 = (center - foot).length();			//圆心到垂足的距离
		if (dis1 > radius)return false;
		float dis2 = sqrt(radius*radius - dis1*dis1);
		inter = foot - d*dis2;
		return true;
	}
	bool Contained(const float &left, const float &right, const float &up, const float &down)
	{
		return(center.x - radius > left) && (center.x + radius < right)
			&& (center.y - radius > up) && (center.y + radius < down);
	}
};