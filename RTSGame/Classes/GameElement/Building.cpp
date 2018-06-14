#include "Building.h"

Building::Building()
{

}

Building::Building(int type)
{
	//�Զ����뽨����𣬽�����Ӧ���Գ�ʼ��
	buildingType = type;
	
	//С�������ɵ�
	if (BuildingType::POWERPLANT)
	{
		spawnPoint = positionCurrent + Vec2(48, -24);
	}
	else spawnPoint = positionCurrent + 2 * Vec2(48, -24);

	//��Ƭ��ͼռλ���λ�ã��ٳ����ĵ�
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

bool Building::readOccupiedTile(bool(*readblock) (Point))
{
	Point middle = this->positionCurrentTM;

	iter = occupiedCoordinate.begin();
	for (iter; iter != occupiedCoordinate.end(); iter++)
	{
		Point temp;
		temp.x = middle.x + (*iter).x;
		temp.y = middle.y + (*iter).y;
		//�ж�������Ƭ���ӵ�Block����
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
		//�ı�������Ƭ���ӵ�Block����
		(*addblock)(temp);
	}
}
