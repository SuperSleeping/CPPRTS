#include "Basement.h"

Basement* Basement::create(Vec2 position)
{
	//����GameElement������Sprite�Ĳ���
	Basement* basement = new (std::nothrow)Basement;
	if (basement && basement->initWithFile("Game/building/basement.png"))
	{
		//ê����������
		basement->setAnchorPoint(Vec2(0.5, 0.3));
		basement->setPosition(position);
		//��������
		basement->positionCurrent = position;
		basement->blood = 200;
		//�Զ��ͷ�
		basement->autorelease();
		return basement;
	}
	CC_SAFE_DELETE(basement);
	return nullptr;
}