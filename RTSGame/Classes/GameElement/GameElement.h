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

};

#endif