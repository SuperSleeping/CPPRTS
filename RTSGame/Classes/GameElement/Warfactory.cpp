#include "Warfactory.h"

Warfactory::Warfactory()
	:Building(WARFACTORY)
{

}

Warfactory* Warfactory::create(Vec2 position)
{
	//����GameElement������Sprite�Ĳ���
	Warfactory* warfactory = new (std::nothrow)Warfactory;
	if (warfactory && warfactory->initWithFile("Game/building/warfactory.png"))
	{
		//ê����������
		warfactory->setAnchorPoint(Vec2(0.5, 0.4));
		warfactory->setPosition(position);
		//��������
		warfactory->blood = 200;
		//�Զ��ͷ�
		warfactory->autorelease();
		return warfactory;
	}
	CC_SAFE_DELETE(warfactory);
	return nullptr;
}
