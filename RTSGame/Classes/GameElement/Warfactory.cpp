#include "Warfactory.h"
#include"GameInformation.h"
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
		warfactory->bloodNow = 200;
		CreateTag++;
		warfactory->setTag(CreateTag);
		//�Զ��ͷ�
		warfactory->autorelease();
		return warfactory;
	}
	CC_SAFE_DELETE(warfactory);
	return nullptr;
}

void Warfactory::setSelected(bool isSelected)
{
	selected = isSelected;
	if (selected)
	{
		this->initWithFile("Game/building/warfactory_selected.png");
		this->setAnchorPoint(Vec2(0.5, 0.4));
	}
	else
	{
		this->initWithFile("Game/building/warfactory.png");
		this->setAnchorPoint(Vec2(0.5, 0.4));
	}
}

