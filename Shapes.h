#pragma once

#include "common.h"

class Shape
{
public:
	Shape() {}
	~Shape() {}
	// �ж��Ƿ��ཻ
	virtual bool Intersect(const Point &p, const Vector2 &d) = 0;
	// �ж��Ƿ��ཻ���󽻵�
	virtual bool Intersect(const Point &p, const Vector2 &d, Point& inter) = 0;
	// �ж��Ƿ���shape�ڲ�
	virtual bool IsInside(const Point &p) = 0;
	// ��ȡ�߽��ϵĵ�����λ�õķ���
	virtual Vector2 GetNormal(const Point &p) = 0;
	// �Ƿ��ڱ߽���
	virtual bool IsOnBoundary(const Point &p) = 0;
	// �Ƿ��ڰ�Χ���ڲ�
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
	bool Intersect(const Point &p, const Vector2 &d)	//��p������d����������Circle�Ƿ��ཻ
	{
		if (IsInside(p)) return true;	//��Բ�ڲ��Ȼ�ཻ
		float proj = (center - p)*d;		//����po�������ϴ���ľ���
		if (proj <= 0) return false;	//��������
		Point foot = p + d * proj;		//����λ��
		float dis = (center - foot).length(); //Բ�ĵ�����ľ���
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
		float proj = (center - p)*d;					//����po�������ϴ���ľ���
		if (proj <= 0) return false;				//��������
		Point foot = p + d * proj;					//����λ��
		float dis1 = (center - foot).length();			//Բ�ĵ�����ľ���
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