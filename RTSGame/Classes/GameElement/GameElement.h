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

	//绑定touch监听器
	EventListenerTouchOneByOne * touchListener;

	//属性
	bool selected = 0;
	int team;
	int prize;
	int blood;

	//血条
	Sprite* bloodFrame;
	Sprite* bloodPicture;

	Vec2 positionCurrent;
	Vec2 positionCurrentTM;

	//选择设置
	void setSelected(bool isSelected)
	{
		selected = isSelected;
	}

	//重载setPosition
	virtual void setPosition(const Vec2& pos);

	Point convertToTM(Point position)
	{
		Point tmSize = Vec2(48,24);
		Point mapSize = Vec2(5664, 3312);
		int x, y;
		x = position.x / tmSize.x;
		y = (mapSize.y - position.y) / tmSize.y;
		return Vec2(x, y);
	}

};

#endif