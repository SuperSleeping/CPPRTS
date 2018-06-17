#include "Building.h"

Building::Building()
{

}

Building::Building(int type)
{
	//自动读入建筑类别，进行相应属性初始化
	buildingType = type;
	
	//小兵等生成点
	spawnPoint = positionCurrent + 2 * Vec2(48, -24);

	//载入血条
	bloodFrame = Sprite::create("Game/building/bloodFrame.png");
	bloodPicture = Sprite::create("Game/building/blood.png");
	bloodPicture->setAnchorPoint(Vec2(0, 0));
	bloodPicture->setPosition(Vec2(1, 1));
	bloodFrame->addChild(bloodPicture);
	bloodFrame->setPositionNormalized(Vec2(0.5, 1.01));
	this->addChild(bloodFrame, 1);
}

//重载setPosition
//				(改动positionCurrent)
//				(spawnPoint点随positionCurrent变动）
void Building::setPosition(const Vec2& pos)
{
	Sprite::setPosition(pos);
	positionCurrent = pos;
	positionCurrentTM = convertToTM(pos);
	spawnPoint += pos;
}

