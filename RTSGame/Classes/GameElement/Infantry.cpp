#include "Infantry.h"

Infantry* Infantry::create(Vec2 position)
{
	//����GameElement������Sprite�Ĳ���
	Infantry* infantry = new (std::nothrow)Infantry;
	if (infantry && infantry->initWithFile("Game/character/infantry/leftdown.png"))
	{
		//ͼƬ����
		infantry->setAnchorPoint(Vec2(0.5, 0));
		infantry->setPosition(position);
		//��ʼλ������
		infantry->setPositionCurrent(position);
		infantry->setPositionGoal(position);
		//��������
		infantry->blood = 125;
		infantry->prize = 120;
		infantry->velocity = 150;
		//�Զ��ͷ�
		infantry->autorelease();
		return infantry;
	}
	CC_SAFE_DELETE(infantry);
	return nullptr;
}