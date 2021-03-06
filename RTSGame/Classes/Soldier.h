#pragma once 
#include"cocos2d.h"
#include"SimpleAudioEngine.h"
#include"Building.h"
#include"Elements.h"
using namespace std;
USING_NS_CC;

float Distance(Vec2 pt1, Vec2 pt2);
class Soldier :public Sprite {
private:
	int side;
	bool selected = 0;
	bool auto_attack = 1;
	//每次攻击间隔时间（秒）
	Soldier* attack_target;
	Building* attack_building;

	int type;
	string type_string;
	float attack_interval;
	float attack_distance = 200;
	int Hitpoint = 100;
	int ATK = 10;

	int count = 0;
	ProgressTimer* blood;
	Vec2 destination;
public:
	static Soldier *create(std::string filename);

	Sprite* FindClosest(Vector<Soldier*> vec);
	Sprite* FindEnemy(Vector<Soldier*> vec);

	void SetType(int typez);
	void SetSide(int side);
	void SetTarget(Soldier* tg);
	void SetDestroy(Building* tg);
	//void SetTarget(Building* tg);
	void update(float di) override;
	void updateAttack(float di);
	void updateBegin(float di);
	void BeAttacked(int damage);
	int Hp();

	bool Died();

	void SetDestination(Vec2 dest);

	void Select(bool select);
	bool Selected();
	void SelectedReply();

	void Refresh();
	//virtual void create();
};
void Soldier::Select(bool select) {
	selected = select;
}
bool Soldier::Selected() {


	return selected;
}
void Soldier::SelectedReply() {
	Sprite* select = Sprite::create("select.png");
	if (selected) {

		this->addChild(select);
		select->setName("select");
		select->setAnchorPoint(Vec2(0, 0));
		//this->setRotation(180.0f);
	}
	else {
		this->removeChildByName("select");
	}
}

Soldier *Soldier::create(std::string filename)
{
	//CreateTag++;
	Soldier *sprite = new Soldier();
	//auto hp = CCProgressTimer::create(Sprite::create("blood.png"));

	if (sprite && sprite->initWithFile(filename))
	{
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);

	return nullptr;


}

void Soldier::SetDestination(Vec2 dest) {
	this->destination = dest;
	this->stopAllActions();
	auto move_target = MoveTo::create(Distance(dest, this->getPosition()) / 50, dest);
	this->runAction(move_target);
}

void Soldier::Refresh() {
	selected = 0;
}
Sprite* Soldier::FindClosest(Vector<Soldier*> vec) {
	Sprite* rtsd;
	float di = 4000 * 4000;
	for (auto sd : vec) {
		if (Distance(sd->getPosition(), this->getPosition()) < di) {
			di = Distance(sd->getPosition(), this->getPosition());
			rtsd = sd;
		}
	}
	return rtsd;
}
Sprite* Soldier::FindEnemy(Vector < Soldier*> vec) {
	for (auto sd : vec) {
		if (Distance(sd->getPosition(), this->getPosition()) < attack_distance) {
			return sd;
		}
	}
	return nullptr;
}

void Soldier::SetType(int typez) {
	switch (typez) {
	case 1:
		break;
	case 2:
		break;
	case 3:
		break;
	}

}

void Soldier::SetSide(int team) {
	this->side = team;
}

void Soldier::SetTarget(Soldier* tg) {
	this->attack_building = nullptr;
	this->attack_target = tg;
}

void Soldier::SetDestroy(Building* tg) {
	this->attack_target = nullptr;
	this->attack_building = tg;
}

void Soldier::BeAttacked(int damage) {
	this->Hitpoint = this->Hitpoint - damage;
	this->blood->setPercentage(Hitpoint);
	if (this->Died()) {
		this->cleanup();
		this->removeAllChildren();
		this->removeFromParent();
	}
}

int Soldier::Hp() {
	return this->Hitpoint;
}

bool Soldier::Died() {
	if (this->Hitpoint <= 0) {
		return 1;
	}
	else {
		return 0;
	}
}
void Soldier::update(float di) {

	if (attack_target) {
		if (attack_target && (Distance(this->getPosition(), attack_target->getPosition()) > attack_distance) && count % 10 == 0) {
			this->stopAllActions();

			this->SetDestination(attack_target->getPosition());

		}
		if (attack_target && (Distance(this->getPosition(), attack_target->getPosition()) <= attack_distance) && count % 10 == 0) {
			this->stopAllActions();

		}
	}
	else if (attack_building) {
		if (attack_building && (Distance(this->getPosition(), attack_building->getPosition()) > attack_distance) && count % 10 == 0) {
			this->stopAllActions();

			this->SetDestination(attack_building->getPosition());

		}
		if (attack_building && (Distance(this->getPosition(), attack_building->getPosition()) <= attack_distance) && count % 10 == 0) {
			this->stopAllActions();

		}
	}
	this->count++;
	//log("%f", Distance(this->getPosition(), attack_target->getPosition()) / 50);


}
void Soldier::updateAttack(float di) {
	if (attack_target) {
		if (attack_target && (Distance(this->getPosition(), attack_target->getPosition()) <= attack_distance)) {
			attack_target->BeAttacked(this->ATK);
			//log("%d", attack_target->Hp());
		}
		if (attack_target && attack_target->Died()) {
			//attack_target->removeFromParent();
			this->SetTarget(nullptr);
		}
	}
	else if (attack_building) {
		log("attack");
		if (attack_building && (Distance(this->getPosition(), attack_building->getPosition()) <= attack_distance)) {
			attack_building->BeAttacked(this->ATK);
			//log("%d", attack_target->Hp());
		}
		if (attack_building && attack_building->Destroyed()) {
			//attack_target->removeFromParent();
			this->SetDestroy(nullptr);
		}
	}

}
void Soldier::updateBegin(float di) {
	CreateTag++;
	this->setTag(CreateTag);
	//log("%d", this->getTag());
	this->blood = ProgressTimer::create(Sprite::create("blood.png"));
	this->addChild(blood);

	blood->setAnchorPoint(Vec2(0, 0));
	blood->setType(ProgressTimer::Type::BAR);
	blood->setMidpoint(Vec2(0, 0.5f));
	blood->setBarChangeRate(Vec2(1, 0));
	blood->setPercentage(100);
}