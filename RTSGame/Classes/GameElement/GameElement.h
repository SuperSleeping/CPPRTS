#ifndef _GAME_ELEMENT_H_
#define _GAME_ELEMENT_H_

#include "cocos2d.h"
#include <string>
USING_NS_CC;

const int DIRECTION[8][2] = {
	{ -1,0 },{ -1,1 },{ 0,1 },{ 1,1 },{ 1,0 },{ 1,-1 } ,{ 0,-1 },{ -1,-1 }
};
static int CreateTag = 0;
static int myTeam = 0;

static int Gold[4] = { 5000,5000,5000,5000 };
static int Power[4] = { 0,0,0,0 };

class GameElement : public Sprite
{
public:
	//����GameElement�ĺ���
	static GameElement* create(const char* filename, Vec2 position);

	//��touch������
	EventListenerTouchOneByOne * touchListener;

	//����
	bool selected=0;
	int team;
	int prize;
	int blood;
	int bloodNow;
	bool died = 0;

	void beAttacked(int damage)
	{
		bloodNow -= damage;
		bloodPicture->setScaleX(bloodNow / blood);
		log("Blood : %d", bloodNow);
		if (bloodNow <= 0)
		{
			died = 1;
		}
	}

	//Ѫ��
	Sprite* bloodFrame;
	Sprite* bloodPicture;

	Vec2 positionCurrent;
	Vec2 positionCurrentTM;

	//ѡ������
	void setSelected(bool isSelected)
	{
		selected = isSelected;
	}

	//����setPosition
	virtual void setPosition(const Vec2& pos);

	Point convertToTM(Point position)
	{
		Point tmSize = Vec2(48,24);
		Point mapSize = Vec2(5664, 3312);
		int x, y;
		x = position.x / tmSize.x;
		y = (mapSize.y - position.y) / tmSize.y;
		return Vec2(x, y);
	}

};

#endif