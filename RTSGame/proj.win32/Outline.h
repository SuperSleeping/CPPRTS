#ifndef _OUTLINT_H_
#define _OUTLINE_H_

#include "cocos2d.h"
USING_NS_CC;

class Outline
{
public:
	Outline(Point a, Point b);
	
	void addto(Layer* layer);
	void removefrom(Layer* layer);

private:
	Point a;
	Point b;

	DrawNode * up;
	Sprite * down;
	Sprite * left;
	Sprite * right;

	enum Mode { rightup=1, rightdown };
	int mode;
	void firstSettingPoint();
	void changePoint();
};

#endif

