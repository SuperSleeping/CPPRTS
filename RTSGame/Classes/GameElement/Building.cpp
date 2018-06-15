#include "Building.h"

Building::Building()
{

}

Building::Building(int type)
{
	//自动读入建筑类别，进行相应属性初始化
	buildingType = type;
	
	//小兵等生成点
	if (buildingType == BuildingType::BARRACK || buildingType == BuildingType::WARFACTORY)
	{
		spawnPoint = Vec2(-48, -24);
	}
	else spawnPoint = Vec2(0, 0);


}

//重载setPosition
//				(改动positionCurrent)
//				(spawnPoint点随positionCurrent变动）
void Building::setPosition(const Vec2& pos)
{
	GameElement::setPosition(pos);
	spawnPoint += pos;
}

