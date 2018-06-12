#pragma once
#include"cocos2d.h"
#include"SimpleAudioEngine.h"
#include"Soldier.h"
#include"Elements.h"
using namespace std;
USING_NS_CC;


class Building :public Sprite {
private:
	int type;
	int side;
	int Hitpoint = 0;
	int power;
	bool power_return = 0;
	float build_progress = 0;
	ProgressTimer* blood;
public:
	static Building *create(std::string filename);
	void SetType(int typez);
	void SetSide(int side);
	int Type();
	int Side();
	void updateBegin(float di);
	void updateBuild(float di);
	void updateOre(float di);
	void BeAttacked(int damage);
	bool Destroyed();
	//virtual void create();
};

void Building::BeAttacked(int damage) {
	this->Hitpoint = this->Hitpoint - damage;
	this->blood->setPercentage(Hitpoint/10);
	if (this->Destroyed()) {
		if (!this->power_return) {
			this->power_return = 1;
			Power[this->side] += this->power;;
		}
		
		this->cleanup();
		this->removeAllChildren();
		this->removeFromParent();
	}
}

int Building::Side() {
	return side;
}


void Building::SetSide(int side) {
	this->side = side;
}

bool Building::Destroyed() {
	if (Hitpoint <= 0) {
		return true;
	}
	return false;
}

Building *Building::create(std::string filename)
{
	//CreateTag++;
	Building *sprite = new Building();

	if (sprite && sprite->initWithFile(filename))
	{
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;


}
void Building::SetType(int typez) {
	this->type = typez;
}

int Building::Type() {
	return this->type;
}

void Building::updateBegin(float di) {
	CreateTag++;
	this->setTag(CreateTag);
	//log("%d", this->getTag());
	this->blood = ProgressTimer::create(Sprite::create("bloodbuild.png"));
	this->addChild(blood);
	this->setOpacity(50);

	switch (this->type){
	case 3:
		Power[this->side] -= 50;
		break;
	case 4:
		Power[this->side] -= 50;
		break;
	case 5:
		Power[this->side] -= 100;
		break;
	}

	blood->setAnchorPoint(Vec2(0, 0));
	blood->setType(ProgressTimer::Type::BAR);
	blood->setMidpoint(Vec2(0, 0.5f));
	blood->setBarChangeRate(Vec2(1, 0));
	blood->setPercentage(0);
}

void Building::updateBuild(float di) {
	this->build_progress++;
	this->blood->setPercentage(build_progress );
	if (build_progress == 100) {
		this->setOpacity(200);
		
		switch (this->type) {
		case 1: 
			ConstructionYard[this->side] = 1;
			this->Hitpoint = 1000;
			break;
		case 2:
			PowerPlant[this->side]++;
			this->Hitpoint = 1000;
			Power[this->side] += 100;
			this->power = -100;
			break;
		case 3:
			OreRefinery[this->side] ++;
			this->Hitpoint = 1000;
			
			this->power = 50;
			break;
		case 4:
			Barracks[this->side] ++;
			this->Hitpoint = 1000;
			
			this->power = 50;
			break;
		case 5:
			WarFactory[this->side] ++;
			this->Hitpoint = 1000;
			
			this->power = 100;
		}
	
	}
}

void Building::updateOre(float di) {
	if (!this->Destroyed()) {
		Gold[this->side] += 50;
	}
}
