#include "baseType.h"

//////////////////////////////////////////
//////positionClass defination.
//////////////////////////////////////////

//positionClass initialized as (0,0)
positionClass::positionClass(float X, float Y)
{
	x = X;
	y = Y;
}

positionClass::~positionClass()
{
	log("~positionClass called.");
}

//using setPosition to change
void positionClass::setPosition(float X, float Y)
{
	x = X;
	y = Y;
}

float positionClass::getX()
{
	return x;
}

float positionClass::getY()
{
	return y;
}


///////////////////////////////////////////
//////

void basetype::stateChange()
{
	if (getAttackedFlag == true)
	{
		getAttacked();
	}
	if (difference_currentPos_targetPos == true)
	{
		movingTo();
	}
}

void basetype::getAttacked()
{
	HP -= damage;
}

//using scheduel function to check out
//moving to targetPos
void basetype::movingTo()
{

}
/*orders*/