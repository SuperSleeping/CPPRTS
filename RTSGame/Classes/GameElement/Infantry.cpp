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
		//��������
		infantry->blood = 125;
		infantry->prize = 120;
		infantry->velocity = 10;
		//�Զ��ͷ�
		infantry->autorelease();
		return infantry;
	}
	CC_SAFE_DELETE(infantry);
	return nullptr;
}