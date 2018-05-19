#ifndef _BASE_TYPE_H_
#define _BASE_TYPE_H_

#include "cocos2d.h"
#include <string>

using namespace std;
USING_NS_CC;

class positionClass
{
public:
	float x;
	float y;

	positionClass(float X = 0, float Y = 0);

	~positionClass();

	void setPosition(float X, float Y);
	float getX();
	float getY();
};

class basetype
{
public:
	string name;

	positionClass currentPos;
	positionClass targetPos;
	
	int price;
	int HP;
	int speed;


	/*interact function*/

	void stateChange();

	void movingTo();
	void getAttacked();




private:
	bool fixed;

	bool difference_currentPos_targetPos;
	
	bool getAttackedFlag;
	int damage;
};




#endif