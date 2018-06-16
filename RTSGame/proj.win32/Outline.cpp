#include "Outline.h"

Outline::Outline(Point a,Point b)
	:a(a),b(b)
{
	firstSettingPoint();
	if (mode!=0)
	{
		if (mode == rightup)
		{

		}
		else if (mode == rightdown)
		{

		}
	}
}

void Outline::firstSettingPoint()
{
	//left-down mode change to right-up mode
	if (b.x < a.x&&b.y < a.y)
	{
		changePoint();
		mode = rightup;
	}
	else if (b.x<a.x&&b.y>a.y)
	{
		changePoint();
		mode = rightdown;
	}
	else if (b.x > a.x&&b.y > a.y)
	{
		mode = rightup;
	}
	else if (b.x > a.x&&b.y < a.y)
	{
		mode = rightdown;
	}
	else
	{
		mode = 0;
	}
}

void Outline::changePoint()
{
	Point c;
	c = b;
	b = a;
	a = c;
}

//appear
void Outline::addto(Layer* layer)
{
	layer->addChild(up);
	layer->addChild(down);
	layer->addChild(left);
	layer->addChild(down);
}

//disappear
void Outline::removefrom(Layer* layer)
{
	layer->removeChild(up);
	layer->removeChild(down);
	layer->removeChild(left);
	layer->removeChild(right);
}