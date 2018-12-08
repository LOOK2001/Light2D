#pragma once
#include <stdlib.h>
#include <vector>
#include "Shapes.h"

using namespace std;

#define MAX_DEPTH 5 // control the maximum recursion depth
#define N 16
#define N2 16
#define TWO_PI	6.28318530718f   // pi
#define BIAS 1e-4f
#define IS_DEBUG true

void drawLine(Point p1, Point p2);

class Atom
{
protected:
	Shape* m_shape;
	Color m_emissive;
	float m_reflectivity;	// 不考虑漫反射
	float m_refractivity;

public:
	Atom(Shape* s, Color e, float re = 0.f) :
		m_shape(s), m_emissive(e), m_reflectivity(re){}
	Shape* GetShape()	{ return m_shape; }
	Color GetEmissive() { return m_emissive; }
	float GetReflectivity() { return m_reflectivity; }
	//判断是否相交并返回交点
	virtual bool Intersect(const Point &p, const Vector2 &d, Point &inter)
	{
		return m_shape->Intersect(p, d, inter);
	}
	//判断是否在包围盒内部
	bool Contained(float left, float right, float up, float down)
	{
		return m_shape->Contained(left, right, up, down);
	}
};

class Scene
{
protected:
	vector<Atom*> m_atoms;

public:
	Scene(vector<Atom*> atoms) :m_atoms(atoms) {}
	~Scene()
	{
		for (auto atom : m_atoms)
			delete atom;
	}

	Color Reflect(Atom* atom, const Point &inter, const Vector2 &dir, int depth)
	{
		Vector2 normal = atom->GetShape()->GetNormal(inter);
		Vector2 refldir = dir.reflect(normal);
		return (Trace(inter + refldir*BIAS, refldir, depth)*atom->GetReflectivity());
	}

	int ii = 0;
	Color Trace(const Point &p,const Vector2 &dir, const int &depth = 0)
	{
		Color trace_emissive{ 0.0f,0.0f,0.0f };
		float distance = 10.0f;
		Atom* nearAtom = nullptr;
		Point inter;

		for (auto atom:m_atoms)
		{
			Point tmpInter;
			if (atom->Intersect(p,dir,tmpInter))
			{
				float newDist = (tmpInter - p).length();
				if (distance>newDist)
				{
					nearAtom = atom;
					distance = newDist;
					inter = tmpInter;
				}
			}
		}

		if (nearAtom)
		{
			if ((nearAtom->GetReflectivity() > 0) && depth < MAX_DEPTH)
			{
				Color reflColor = Reflect(nearAtom, inter, dir, depth+1);
				return nearAtom->GetEmissive()+ reflColor;
			}
			else
			{
				return nearAtom->GetEmissive();
			}
		}
		else
			return{ 0.0f,0.0f,0.0f };
	}

	Color Sample(const Point &p)
	{
		Color sum;
#pragma omp parallel for
		for (int i = 0; i < N; i++)
		{
			float a = TWO_PI * (i + (float)rand() / RAND_MAX) / N;
			sum = sum + Trace(p, Vector2( cosf(a), sinf(a) ));
		}
		return sum / N;
	}
};