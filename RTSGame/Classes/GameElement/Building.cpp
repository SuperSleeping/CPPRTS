#include "Building.h"
#include"GameInformation.h"
extern void BuildBlock(int x, int y, int size);

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

