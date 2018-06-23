#include "Building.h"
#include"GameInformation.h"
extern void BuildBlock(int x, int y, int size);

Building::Building()
{

}

Building::Building(int type)
{
	//�Զ����뽨����𣬽�����Ӧ���Գ�ʼ��
	buildingType = type;
	
	//С�������ɵ�
	spawnPoint = positionCurrent + 2 * Vec2(48, -24);

	//����Ѫ��
	bloodFrame = Sprite::create("Game/blood/buildingBlood/bloodFrame.png");
	bloodPicture = ProgressTimer::create(Sprite::create("Game/blood/buildingBlood/blood1.png"));
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
	bloodFrame->setPositionNormalized(Vec2(0.5, 1.01));
	this->addChild(bloodFrame, 1);
}

//����setPosition
//				(�Ķ�positionCurrent)
//				(spawnPoint����positionCurrent�䶯��
void Building::setPosition(const Vec2& pos)
{
	Sprite::setPosition(pos);
	positionCurrent = pos;
	positionCurrentTM = convertToTM(pos);
	spawnPoint += pos;
}

