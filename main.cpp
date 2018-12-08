#include "svpng.inc"
#include "common.h"
#include "Scene.h"
#include <math.h>
#include <time.h>
#include <vector>
#include <iostream>

using namespace std;

#define W 512
#define H 512

unsigned char img[W * H * 3];

typedef struct 
{ 
	float sd;
	Color emissive; 
} Result;

Scene* CreateNewScene()
{
	vector<Atom*> newScene;
	Circle* c1 = new Circle(Point(0.5f, 0.6f), 0.1f);
	Circle* c2 = new Circle(Point(0.3f, 0.8f), 0.12f);	
	Circle* c3 = new Circle(Point(0.2f, 0.4f), 0.06f);
	Circle* c4 = new Circle(Point(0.25f, 0.15f), 0.055f);

	Atom* atom1 = new Atom(c1, Color(1.8f, 0.9f, 0.7f), 0.0f);
	Atom* atom2 = new Atom(c2, Color(0.2f, 0.9f, 1.1f ), 0.0f);
	Atom* atom3 = new Atom(c3, Color(0.05f, 0.05f, 0.2f), 0.8f);
	Atom* atom4 = new Atom(c4, Color(0.05f, 0.05f, 0.2f), 0.8f);

	newScene.push_back(atom1);
	newScene.push_back(atom2);
	newScene.push_back(atom3);
	newScene.push_back(atom4);

	return new Scene(newScene);
}

void render(Scene *scene)
{
	unsigned char* p = img;
	Color color;
	int loading = 1;
	for (int y = 0; y < H; y++)
	{
		for (int x = 0; x < W; x++, p += 3)
		{
			color = scene->Sample(Point( (float)x / W, (float)y / H ));
			color.r = color.r > 0.0f ? color.r : 0.0f;
			color.g = color.g > 0.0f ? color.g : 0.0f;
			color.b = color.b > 0.0f ? color.b : 0.0f;
			p[0] = (int)(fminf((color.r * 255.0f), 255.0f));
			p[1] = (int)(fminf((color.g * 255.0f), 255.0f));
			p[2] = (int)(fminf((color.b * 255.0f), 255.0f));
		}

		if (y > (H*0.01*loading))
			std::cout << loading++ << "%" << std::endl;
	}
	svpng(fopen("basic.png", "wb"), W, H, img, 0);
}

int main()
{
	clock_t totalTime = clock();

	Scene* newScene = CreateNewScene();
	render(newScene);

	totalTime = clock() - totalTime;
	std::cout << totalTime / CLOCKS_PER_SEC << "s" <<std::endl;
}