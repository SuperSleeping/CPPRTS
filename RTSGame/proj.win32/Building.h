#pragma once
#include"cocos2d.h"
#include"SimpleAudioEngine.h"
#include"Soldier.h"
using namespace std;
USING_NS_CC;

class Building :public Sprite {
private:
	int type;
	int side;
public:
	static Building *create(std::string filename);
	void SetType(int typez);
	void GiveSide(int side);
	int Type();
	int Side();
	//virtual void create();
};

int Building::Side() {
	return side;
}


void Building::GiveSide(int side) {
	this->side = side;
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