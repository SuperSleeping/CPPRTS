#include "Infantry.h"

Infantry* Infantry::create(Vec2 position)
{
	//����GameElement������Sprite�Ĳ���
	Infantry* infantry = new (std::nothrow)Infantry;
	if (infantry && infantry->initWithFile("Game/character/infantry/leftdown.png"))
	{
		infantry->setAnchorPoint(Vec2(0.5, 0));
		infantry->setPosition(position);
		infantry->positionCurrent = position;
		infantry->setGoal(convertToTiledMap(position)+Vec2(0,2));
		//��������
		infantry->blood = 125;
		infantry->bloodNow = 125;
		infantry->attack = 10;
		infantry->attackDistance = 3;
		infantry->prize = 120;
		infantry->velocity = 10;
		//�Զ��ͷ�
		CreateTag++;
		infantry->setTag(CreateTag);

		infantry->schedule(schedule_selector(Character::updateMove), 0.01f, kRepeatForever, 0.0f);
		infantry->schedule(schedule_selector(Character::updateAttack), 0.1f, kRepeatForever, 0.0f);


		infantry->autorelease();
		return infantry;
	}
	CC_SAFE_DELETE(infantry);
	return nullptr;
}