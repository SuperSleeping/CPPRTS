#include "Barrack.h"


Barrack* Barrack::create(Vec2 position)
{
	//����GameElement������Sprite�Ĳ���
	Barrack* barrack = new (std::nothrow)Barrack;
	if (barrack && barrack->initWithFile("Game/building/barrack.png"))
	{
		//ê����������
		barrack->setAnchorPoint(Vec2(0.5, 0.5));
		barrack->setPosition(position);
		//��������
		barrack->positionCurrent = position;
		barrack->blood = 200;
		//�Զ��ͷ�
		barrack->autorelease();
		return barrack;
	}
	CC_SAFE_DELETE(barrack);
	return nullptr;
}