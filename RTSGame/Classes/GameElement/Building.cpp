#include "Building.h"

Building::Building()
{

}

Building::Building(int type)
{
	//自动读入建筑类别，进行相应属性初始化
	buildingType = type;
	
	//小兵等生成点
	if (BuildingType::POWERPLANT)
	{
		spawnPoint = positionCurrent + Vec2(48, -24);
	}
	else spawnPoint = positionCurrent + 2 * Vec2(48, -24);

	//瓦片地图占位相对位置（刨除中心点
	if (BuildingType::POWERPLANT)
	{
		//      *
		//    * 0 *
		//      *
		occupiedCoordinate.push_back(Vec2(-1, 0));
		occupiedCoordinate.push_back(Vec2(0, -1));
		occupiedCoordinate.push_back(Vec2(0, 0));
		occupiedCoordinate.push_back(Vec2(0, 1));
		occupiedCoordinate.push_back(Vec2(0, 1));
	}
	else
	{
		//      *
		//    * * *
		//  * * 0 * *
		//    * * *
		//      *
		occupiedCoordinate.push_back(Vec2(-2, 0));
		occupiedCoordinate.push_back(Vec2(-1, -1));
		occupiedCoordinate.push_back(Vec2(-1, 0));
		occupiedCoordinate.push_back(Vec2(-1, 1));
		occupiedCoordinate.push_back(Vec2(0, -2));
		occupiedCoordinate.push_back(Vec2(0, -1));
		occupiedCoordinate.push_back(Vec2(0, 1));
		occupiedCoordinate.push_back(Vec2(0, 2));
		occupiedCoordinate.push_back(Vec2(1, -1));
		occupiedCoordinate.push_back(Vec2(1, 0));
		occupiedCoordinate.push_back(Vec2(1, 1));
		occupiedCoordinate.push_back(Vec2(2, 2));
	}
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

bool Building::readOccupiedTile(bool(*readblock) (Point))
{
	Point middle = this->positionCurrentTM;

	iter = occupiedCoordinate.begin();
	for (iter; iter != occupiedCoordinate.end(); iter++)
	{
		Point temp;
		temp.x = middle.x + (*iter).x;
		temp.y = middle.y + (*iter).y;
		//判断所在瓦片格子的Block属性
		if ((*readblock)(temp))
		{
			return true;
		}
	}
	return false;
}

void Building::changeOccupiedTile(void(*addblock) (Point))
{
	Point middle = this->positionCurrentTM;

	iter = occupiedCoordinate.begin();
	for (iter; iter != occupiedCoordinate.end(); iter++)
	{
		Point temp;
		temp.x = middle.x + (*iter).x;
		temp.y = middle.y + (*iter).y;
		//改变所在瓦片格子的Block属性
		(*addblock)(temp);
	}
}
