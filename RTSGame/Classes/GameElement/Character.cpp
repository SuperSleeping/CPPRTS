#include "Character.h"

Character* Character::create(const char* filename, Vec2 position)
{
	//创建GameElement中类似Sprite的部分
	Character* character = new (std::nothrow)Character;
	if (character && character->initWithFile(filename))
	{
		character->setPosition(position);
		//属性设置
		character->positionCurrent = position;
		//自动释放
		character->autorelease();
		return character;
	}
	CC_SAFE_DELETE(character);
}

void Character::move()
{
	//判断该不该走
	if (positionGoal == NULL ||
		positionCurrent == positionGoal)return;

	//移动
	float _distance = distance(positionCurrent, positionGoal);
	float time = _distance / velocity;
	
	auto moveToAction = MoveTo::create(time, positionGoal);
	this->runAction(moveToAction);
}

float Character::distance(Vec2 a, Vec2 b)
{
	float _distance = sqrt(
		abs(
		(a.x - b.x)*(a.x - b.x) +
			(a.y - b.y)*(a.y - b.y)
		)
	);
	return _distance;
}