#include "Minefield.h"
#include"GameInformation.h"
Minefield::Minefield()
	:Building(MINEFIELD)
{

}

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
		minefield->bloodNow = 200;
		CreateTag++;
		minefield->setTag(CreateTag);
		//�Զ��ͷ�
		minefield->autorelease();
		return minefield;
	}
	CC_SAFE_DELETE(minefield);
	return nullptr;
}

void Minefield::setSelected(bool isSelected)
{
	selected = isSelected;
	if (selected)
	{
		this->initWithFile("Game/building/minefield_selected.png");
		this->setAnchorPoint(Vec2(0.5, 0.25));
	}
	else
	{
		this->initWithFile("Game/building/minefield.png");
		this->setAnchorPoint(Vec2(0.5, 0.25));
	}
}