#include "Tank.h"
#include"GameInformation.h"
extern int CreateTag;

Tank* Tank::create(Vec2 position)
{
	//����GameElement������Sprite�Ĳ���
	Tank* tank = new (std::nothrow)Tank;
	if (tank && tank->initWithFile("Game/character/tank/leftdown.png"))
	{
		//λ������
		tank->setAnchorPoint(Vec2(0.5, 0));
		tank->setPosition(position);
		tank->positionCurrent = position;
		tank->setGoal(convertToTiledMap(position) + Vec2(0, 2));

		//��������
		tank->blood = 170;
		tank->bloodNow = 170;
		tank->attack = 20;
		tank->attackDistance = 5;
		tank->prize = 250;
		tank->velocity = 15;

		tank->schedule(schedule_selector(Character::updateMove), 0.01f, kRepeatForever, 0.0f);
		tank->schedule(schedule_selector(Character::updateAttack), 0.1f, kRepeatForever, 0.0f);
		//�Զ��ͷ�
		tank->autorelease();
		return tank;
	}
	CC_SAFE_DELETE(tank);
	return nullptr;
}