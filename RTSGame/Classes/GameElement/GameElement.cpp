#include"GameElement.h"
#include<math.h>



/***********************
      CONSTRUCTORS
************************/

//��������GameElement��ָ�룬ָ����new������һ��GameElement���󣬱����ڲ�ʹ�õ�ʱ����delete�ͷſռ䡣
GameElement* GameElement::create(const char* filename, Vec2 position)
{
	//Use new to create a inherited class pointer from (Node*)
	GameElement* Ge = new GameElement;

	//Create a picture to present the sprite
	Ge->picture = Sprite::create(filename);
//	ge->picture->setAnchorPoint(Vec2(ge->picture->getContentSize().width / 2, ge->picture->getContentSize().height / 2));
	Ge->picture->setPosition(position);

	//Default velocity
	Ge->velocity = 10;
	
	Ge->autorelease();

	return Ge;
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
	//�жϸò�����
	if (positionCurrent == positionGoal)return;
	
	//�ƶ�
	float _distance = distance(positionCurrent, positionGoal);
	float time = _distance / velocity;
	///////////////////////
	//�ȼ���movetoд�ˣ�//
	///////////////////////
	auto moveToAction = MoveTo::create(time, positionGoal);
//	picture->setPosition(positionGoal);
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