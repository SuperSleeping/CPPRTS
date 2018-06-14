#include "Building.h"

Building::Building()
{

}

Building::Building(int type)
{
	buildingType = type;
	if (BuildingType::POWERPLANT)
	{
		spawnPoint = positionCurrent + Vec2(48, -24);
	}
	else spawnPoint = positionCurrent + 2 * Vec2(48, -24);
}

//����setPosition
//				(�Ķ�positionCurrent)
//				(spawnPoint����positionCurrent�䶯��
void Building::setPosition(const Vec2& pos)
{
	Sprite::setPosition(pos);
	positionCurrent = pos;
	spawnPoint += pos;
}