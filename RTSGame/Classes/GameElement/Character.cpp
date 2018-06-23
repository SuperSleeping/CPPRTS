#include "Character.h"
#include"GameInformation.h"
Character::Character()
{
	//����ѡ�б��
	shadow = Sprite::create("Game/rhombus.png");
	shadow->setPositionNormalized(Vec2(0.5, 0));
	shadow->setVisible(false);
	this->addChild(shadow, -1);
	//����Ѫ��
	bloodFrame = Sprite::create("Game/blood/characterBlood/bloodFrame.png");
		bloodPicture = ProgressTimer::create(Sprite::create("Game/blood/characterBlood/blood1.png"));
		//bloodPicture = Sprite::create("Game/character/blood.png");
	//bloodPicture->setAnchorPoint(Vec2(0, 0));
	bloodPicture->setPosition(Vec2(1, 1));

	//this->addChild(blood);

	bloodPicture->setAnchorPoint(Vec2(0, 0));
	bloodPicture->setType(ProgressTimer::Type::BAR);
	bloodPicture->setMidpoint(Vec2(0, 0.5f));
	bloodPicture->setBarChangeRate(Vec2(1, 0));
	bloodPicture->setPercentage(100);
	bloodFrame->addChild(bloodPicture);
	bloodFrame->setPositionNormalized(Vec2(0.5, 1.2));
	this->addChild(bloodFrame,1);
}

Character* Character::create(const char* filename, Vec2 position,int tm)
{
	//����GameElement������Sprite�Ĳ���
	Character* character = new (std::nothrow)Character;
	if (character && character->initWithFile(filename))
	{
		character->team = tm;
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

void Character::setSelected(bool isSelected)
{
	if (isSelected)
	{
		this->selected = true;
		this->shadow->setVisible(true);
	}
	else
	{
		this->selected = false;
		this->shadow->setVisible(false);
	}
}