#ifndef _CHARACTER_H_
#define _CHARACTER_H_

#include "GameElement.h"

extern int MapInfo[118][138];
extern int Block[118][138];
extern int Characters[118][138];
extern int Buildings[118][138];

extern Point convertToTiledMap(Point pt);
extern Point convertFromTMToWorld(Point pt);


class Character : public GameElement
{
public:
	Character();

	//Create Character
	static Character* create(const char* filename, Vec2 position);

	//人物类型
	enum CharacterType
	{
		INFANTRY = 6,
		DOG,
		TANK,
		CharacterChosen
	};

	//选中标记
	Sprite* shadow;
	virtual void setSelected(bool isSelected);

	//运动相关
	Vec2 positionGoal;
	Vec2 positionNow;
	Vec2 positionTarget;
	float velocity;
	//检测目标位置与当前位置后进行移动
	void move();
	
	void setTeam(int tm)
	{
		team = tm;
		bloodFrame = Sprite::create("Game/blood/characterBlood/bloodFrame.png");
		{
			switch (tm)
			{
			case 0:bloodPicture = ProgressTimer::create(Sprite::create("Game/blood/characterBlood/blood2.png"));
				break;
			case 1:bloodPicture = ProgressTimer::create(Sprite::create("Game/blood/characterBlood/blood1.png"));
				break;
			case 2:bloodPicture = ProgressTimer::create(Sprite::create("Game/blood/characterBlood/blood3.png"));
				break;
			case 3:bloodPicture = ProgressTimer::create(Sprite::create("Game/blood/characterBlood/blood4.png"));
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
			bloodFrame->setPositionNormalized(Vec2(0.5, 1.2));
			this->addChild(bloodFrame, 1);
		}
	}

	void beAttacked(int damage)
	{
		bloodNow -= damage;
		bloodPicture->setPercentage(bloodNow/blood*100);
		log("Blood : %f", bloodNow);
		if (bloodNow <= 0)
		{
			died = 1;
			this->selected = 0;
			//this->setZOrder(-100);
			//log("%f",this->getZOrder());
			this->setZOrder(-1);
			//this->removeAllChildren();
			//this->initWithFile("remove.png");
		}
	}

	void setGoal(Vec2 goal) {
		this->stop = 0;
		positionGoal = goal;
		setMapDestination(goal);
	}

	int attack;
	int attackTag = 0;
	int attackInterval = 0;
	int attackDistance;
	int formerValue;

	bool stop = 0;

	int repeat = 0;
	void setAttackTag(int tag)
	{
		this->attackTag = tag;
		this->attackInterval = 0;
	}
	void updateMove(float di);
	void updateAttack(float di);

	int MapDestination[118][138];

	void setMapDestination(Point pt)
	{
		int x = pt.x;
		int y = pt.y;
		int q = 5;					//权重值

		for (int i = 0; i < 118; i++)
		{
			for (int j = 0; j < 138; j++)
			{
				MapDestination[i][j] = 0;
			}
		}

		MapDestination[x][y] = 700;				//确保所有辐射值都为正数
		for (int i = x; i >= 0; i--)
		{
			MapDestination[i][y] = 700 - q * (x - i);
			for (int j = y - 1; j >= 0; j--)
			{
				MapDestination[i][j] = MapDestination[i][y] - q * (y - j);
			}
			for (int j = y + 1; j < 138; j++)
			{
				MapDestination[i][j] = MapDestination[i][y] - q * (j - y);
			}
		}
		for (int i = x; i < 118; i++)
		{
			MapDestination[i][y] = 700 - q * (i - x);
			for (int j = y - 1; j >= 0; j--)
			{
				MapDestination[i][j] = MapDestination[i][y] - q * (y - j);
			}
			for (int j = y + 1; j < 138; j++)
			{
				MapDestination[i][j] = MapDestination[i][y] - q * (j - y);
			}
		}
	}

private:


	float distance(Vec2 a, Vec2 b)
	{
		float _distance = sqrt(
			abs(
			(a.x - b.x)*(a.x - b.x) +
				(a.y - b.y)*(a.y - b.y)
			)
		);
		return _distance;
	}
};


#endif