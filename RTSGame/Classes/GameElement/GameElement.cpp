#include"GameElement.h"
#include<math.h>



/***********************
      CONSTRUCTORS
************************/
//
GameElement* GameElement::create(const char* filename, Vec2 position)
{
	//use new to create a inherited class pointer from (Node*)
	GameElement* ge = new GameElement();

	//create a picture to present the sprite
	ge->picture = Sprite::create(filename);
	ge->picture->setAnchorPoint(Vec2(ge->picture->getContentSize().width / 2, ge->picture->getContentSize().height / 2));
	ge->picture->setPosition(position);

	return ge;
}

void GameElement::setVelocity(int _velocity)
{
	velocity = _velocity;
}

void GameElement::setPositionGoal(float x, float y)
{
	positionGoal.x = x;
	positionGoal.y = y;
}

void GameElement::move()
{
	//judge whether to move or not.
	if (positionCurrent == positionGoal)return;
	
	//direction
	float _distance = distance(positionCurrent, positionGoal);
	float time = _distance / velocity;
	///////////////////////
	//先简单用moveto写了！//
	///////////////////////
	auto moveToAction = MoveTo::create(time, positionGoal);
	picture->runAction(moveToAction);
}

float GameElement::distance(Vec2 a, Vec2 b)
{
	float _distance = sqrt(
					  abs(
					(a.x - b.x)*(a.x - b.x) +
					(a.y - b.y)*(a.y - b.y)
					)
					);
	return _distance;
}