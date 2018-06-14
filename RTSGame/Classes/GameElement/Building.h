#ifndef _BUILDING_H_
#define _BUILDING_H_

#include"GameElement.h"

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
	int electricity;
	Point spawnPoint;

	//Construction:
	Building();
	Building(int type);

	virtual void setPosition(const Vec2& pos);
};

#endif