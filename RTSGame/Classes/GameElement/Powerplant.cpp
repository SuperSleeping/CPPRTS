#include "Powerplant.h"
#include"GameInformation.h"
Powerplant::Powerplant()
	:Building(POWERPLANT)
{

}

Powerplant* Powerplant::create(Vec2 position)
{
	//����GameElement������Sprite�Ĳ���
	Powerplant* powerplant = new (std::nothrow)Powerplant;
	if (powerplant && powerplant->initWithFile("Game/building/powerplant.png"))
	{
		//ê����������
		powerplant->setAnchorPoint(Vec2(0.5f, 0.2f));
		powerplant->setPosition(position);
		//��������
		powerplant->selected = 0;
		powerplant->positionCurrent = position;
		powerplant->blood = 2000;
		powerplant->bloodNow = 2000;
		CreateTag++;
		powerplant->setTag(CreateTag);
		//�Զ��ͷ�
		powerplant->autorelease();
		return powerplant;
	}
	CC_SAFE_DELETE(powerplant);
	return nullptr;
}

void Powerplant::setSelected(bool isSelected)
{
	selected = isSelected;
	if (selected)
	{
		this->initWithFile("Game/building/powerplant_selected.png");
		this->setAnchorPoint(Vec2(0.5f, 0.2f));
	}
	else
	{
		this->initWithFile("Game/building/powerplant.png");
		this->setAnchorPoint(Vec2(0.5f, 0.2f));
	}
}