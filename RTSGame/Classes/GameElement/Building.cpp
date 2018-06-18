#include "Building.h"

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
	bloodFrame = Sprite::create("Game/building/bloodFrame.png");
	bloodPicture = Sprite::create("Game/building/blood.png");
	bloodPicture->setAnchorPoint(Vec2(0, 0));
	bloodPicture->setPosition(Vec2(1, 1));
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

