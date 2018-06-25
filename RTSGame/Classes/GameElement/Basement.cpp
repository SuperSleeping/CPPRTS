#include "Basement.h"
#include"GameInformation.h"
Basement::Basement()
	:Building(BASEMENT)
{

}

Basement* Basement::create(Vec2 position)
{
	//����GameElement������Sprite�Ĳ���
	Basement* basement = new (std::nothrow)Basement;
	if (basement && basement->initWithFile("Game/building/basement.png"))
	{
		//ê����������
		basement->setAnchorPoint(Vec2(0.5f, 0.3f));
		basement->setPosition(position);
		//��������
		basement->blood = 2000;
		basement->bloodNow = 2000;
		CreateTag++;
		basement->setTag(CreateTag);
		//�Զ��ͷ�
		basement->autorelease();
		return basement;
	}
	CC_SAFE_DELETE(basement);
	return nullptr;
}

void Basement::setSelected(bool isSelected)
{
	selected = isSelected;
	if (selected)
	{
		this->initWithFile("Game/building/basement_selected.png");
		this->setAnchorPoint(Vec2(0.5f, 0.3f));
	}
	else
	{
		this->initWithFile("Game/building/basement.png");
		this->setAnchorPoint(Vec2(0.5f, 0.3f));
	}
}