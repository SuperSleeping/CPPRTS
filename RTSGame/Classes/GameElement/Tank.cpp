#include "Tank.h"

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
		//��������
		tank->blood = 170;
		tank->bloodNow = 170;
		tank->prize = 250;
		tank->velocity = 15;
		CreateTag++;
		tank->setTag(CreateTag);
		//�Զ��ͷ�
		tank->autorelease();
		return tank;
	}
	CC_SAFE_DELETE(tank);
	return nullptr;
}