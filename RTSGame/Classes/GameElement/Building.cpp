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

