#ifndef _GAME_ELEMENT_H_
#define _GAME_ELEMENT_H_

#include "cocos2d.h"
#include <string>
USING_NS_CC;



class GameElement : public Node
{
public:
	//Name&Number
	int number;

	//Constructor
	static GameElement* create(const char* filename, Vec2 position);

	//Sprite
	Sprite* picture;

	//State
	bool selected;

	/********************
	    INSTRUCTIONS
	********************/
	//Moving about:
	int velocity;
	void setVelocity(int _velocity);
	
	Vec2 positionCurrent;
	Vec2 positionGoal;
	void setPositionGoal(float x,float y);
	void move();

private:
	float distance(Vec2 a, Vec2 b);
};

#endif