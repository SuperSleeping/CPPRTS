#include "Character.h"

Character* Character::create(const char* filename, Vec2 position)
{
	//����GameElement������Sprite�Ĳ���
	Character* character = new (std::nothrow)Character;
	if (character && character->initWithFile(filename))
	{
		character->setPosition(position);
		//��������
		character->positionCurrent = position;
		//�Զ��ͷ�
		character->autorelease();
		return character;
	}
	CC_SAFE_DELETE(character);
	return nullptr;
}

void Character::move()
{
	//�жϸò�����
	if (positionGoal == NULL ||
		positionCurrent == positionGoal)return;

	//�ƶ�
	float _distance = distance(positionCurrent, positionGoal);
	float time = _distance / velocity;
	
	auto moveToAction = MoveTo::create(time, positionGoal);
	this->runAction(moveToAction);
}
