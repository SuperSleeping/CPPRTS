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

	void setTeam(int tm)
	{
		team = tm;
		bloodFrame = Sprite::create("Game/blood/buildingBlood/bloodFrame.png");
		{
			switch (tm)
			{
			case 0:bloodPicture = ProgressTimer::create(Sprite::create("Game/blood/buildingBlood/blood2.png"));
				break;
			case 1: bloodPicture = ProgressTimer::create(Sprite::create("Game/blood/buildingBlood/blood1.png"));
				break;
			case 2:bloodPicture = ProgressTimer::create(Sprite::create("Game/blood/buildingBlood/blood3.png"));
				break;
			case 3:bloodPicture = ProgressTimer::create(Sprite::create("Game/blood/buildingBlood/blood4.png"));
				break;
			}


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
	}

	void beAttacked(int damage)
	{
		bloodNow -= damage;
		bloodPicture->setPercentage(bloodNow / blood * 100);
		log("Blood : %f   %f", bloodNow, blood);
		log("perc %f", bloodNow / blood * 100);
		if (bloodNow <= 0)
		{
			died = 1;
			this->selected = 0;
			bloodFrame->setVisible(false);
			//this->setZOrder(-100);
			//log("%f",this->getZOrder());
			//this->removeAllChildren();
			this->initWithFile("Game/building/diedbuilding.png");
		}
	}

	virtual void setPosition(const Vec2& pos);

private:

};

#endif