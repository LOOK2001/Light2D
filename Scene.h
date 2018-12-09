#pragma once
#include <stdlib.h>
#include <vector>
#include "Shapes.h"

using namespace std;

#define MAX_DEPTH 5 // control the maximum recursion depth
#define N 64
#define TWO_PI	6.28318530718f   // pi
#define BIAS 1e-4f

class Atom
{
protected:
	Shape* m_shape;
	Color m_emissive;
	float m_reflectivity;

public:
	Atom(Shape* s, Color e, float re = 0.f) :
		m_shape(s), m_emissive(e), m_reflectivity(re){}
	Shape* GetShape()	{ return m_shape; }
	Color GetEmissive() { return m_emissive; }
	float GetReflectivity() { return m_reflectivity; }
	// if the ray intersect with atom and return the intersection
	virtual bool Intersect(const Point &p, const Vector2 &d, Point &inter)
	{
		return m_shape->Intersect(p, d, inter);
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
		// compute the normal of the point and get the direction
		Vector2 normal = atom->GetShape()->GetNormal(inter);
		Vector2 refldir = dir.reflect(normal);
		normal = normal.normalize();
		return (Trace(inter + refldir*BIAS, refldir, depth)*atom->GetReflectivity());
	}

	// ray-tracing method
	Color Trace(const Point &p,const Vector2 &dir, const int &depth = 0)
	{
		float distance = 10.0f;
		Atom* nearAtom = nullptr;
		Point inter;

		for (auto atom:m_atoms)
		{
			Point tmpInter;
			if (atom->Intersect(p,dir,tmpInter))
			{
				// the ray intersect with a atom, then record the atom and 
				// the distance between intersection and origin of ray
				float newDist = (tmpInter - p).length();
				if (newDist<distance)
				{
					nearAtom = atom;
					distance = newDist;
					inter = tmpInter;
				}
			}
		}

		if (nearAtom)
		{
			// if the ray intersect with a atom, then compute the reflect ray 
			// and trace again
			if ((nearAtom->GetReflectivity() > 0) && depth < MAX_DEPTH)
			{
				Color reflColor = Reflect(nearAtom, inter, dir, depth+1);
				return nearAtom->GetEmissive()+ reflColor;
			}
			else
				return nearAtom->GetEmissive();
		}
		// if there are no intersection, then return background color
		else
			return{ 0.0f,0.0f,0.0f };
	}

	Color Sample(const Point &p)
	{
		Color sum;
		// Jittered sampling (N samples per pixel)
		for (int i = 0; i < N; i++)
		{
			float a = TWO_PI * (i + (float)rand() / RAND_MAX) / N;
			sum = sum + Trace(p, Vector2( cosf(a), sinf(a) ));
		}
		return sum / N;
	}
};