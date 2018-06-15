#ifndef _GAME_ELEMENT_H_
#define _GAME_ELEMENT_H_

#include "cocos2d.h"
#include <string>
USING_NS_CC;


class GameElement : public Sprite
{
public:
	//创建GameElement的函数
	static GameElement* create(const char* filename, Vec2 position);

	//属性
	bool selected;
	int team;
	int prize;
	int blood;

	Vec2 positionCurrent;
	Vec2 positionCurrentTM;

	//重载setPosition()
	void setPosition(const Vec2& pos);

private:
	Point convertToTM(Point position)
	{
		Point mapSize = Vec2(5664, 3312);
		Point tmSize = Vec2(48, 24);

		int x, y;
		x = position.x / tmSize.x;
		y = (mapSize.y - position.y) / tmSize.y;
		return Vec2(x, y);
	}
};

#endif