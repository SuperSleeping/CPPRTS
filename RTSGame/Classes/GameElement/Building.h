#ifndef _BUILDING_H_
#define _BUILDING_H_

#include<vector>
#include"GameElement.h"

using std::vector;

class Building : public GameElement
{
public:
	enum BuildingType
	{
		BASEMENT = 1,
		POWERPLANT,
		MINEFIELD,
		BARRACK,
		WARFACTORY
	};
	int buildingType;
	Point spawnPoint;

	//����
	int electricity;

	//Construction:
	Building();
	Building(int type);

	virtual void setPosition(const Vec2& pos);

private:
	Point convertToTM(Point position)
	{
		Point mapSize = Vec2(5664, 3312);
		Point tmSize = Vec2(48, 24);

		position.y = mapSize.y - position.y;

		Point tmPoint;
		int addx = 0, addy = 0;
		if ((int)position.x % (int)tmSize.x > tmSize.x / 2) addx++;
		if ((int)position.y % (int)tmSize.y > tmSize.y / 2) addy++;
		tmPoint.x = (int)(position.x / tmSize.x) + addx;
		tmPoint.y = (int)(position.y / tmSize.y) - addy;
		return tmPoint;
	}
};

#endif