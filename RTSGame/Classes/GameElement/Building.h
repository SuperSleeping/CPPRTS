#ifndef _BUILDING_H_
#define _BUILDING_H_

#include<vector>
#include"GameElement.h"
#include"GameInformation.h"
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

	//ÊôÐÔ
	int electricity;

	//Construction:
	Building();
	Building(int type);

	void beAttacked(int damage)
	{
		bloodNow -= damage;
		bloodPicture->setPercentage(bloodNow / blood * 100);
		log("Blood : %d   %d", bloodNow,blood);
		log("perc %f", bloodNow / blood * 100);
		if (bloodNow <= 0)
		{
			died = 1;
			this->selected = 0;
			//this->setZOrder(-100);
			//log("%f",this->getZOrder());
			this->removeAllChildren();
			this->initWithFile("Game/building/diedbuilding.png");
		}
	}

	virtual void setPosition(const Vec2& pos);

private:

};

#endif