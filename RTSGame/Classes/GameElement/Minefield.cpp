#include "Minefield.h"

Minefield* Minefield::create(Vec2 position)
{
	//����GameElement������Sprite�Ĳ���
	Minefield* minefield = new (std::nothrow)Minefield;
	if (minefield && minefield->initWithFile("Game/building/minefield.png"))
	{
		//ê����������
		minefield->setAnchorPoint(Vec2(0.5, 0.25));
		minefield->setPosition(position);
		//��������
		minefield->positionCurrent = position;
		minefield->blood = 200;
		//�Զ��ͷ�
		minefield->autorelease();
		return minefield;
	}
	CC_SAFE_DELETE(minefield);
	return nullptr;
}