#include "Barrack.h"
#include"GameInformation.h"
extern void BuildBlock(int x, int y, int size);
//extern int Buildings[118][138];
extern Point convertToTiledMap(Point pt);
extern Point convertFromTMToWorld(Point pt);

Barrack::Barrack()
	:Building(BARRACK)
{
	this->initWithFile("Game/building/barrack.png");
	this->setAnchorPoint(Vec2(0.5f, 0.4f));
	this->blood = 2000;
	this->bloodNow = 2000;
	
	
	

}

Barrack* Barrack::create(Vec2 position)
{
	Barrack* barrack = new (std::nothrow)Barrack;
	if (barrack)
	{
		barrack->setPosition(position);
		barrack->positionCurrent = position;
		//�Զ��ͷ�
		barrack->autorelease();
		CreateTag++;
		barrack->setTag(CreateTag);
		return barrack;
	}
	CC_SAFE_DELETE(barrack);
	return nullptr;
}

void Barrack::setSelected(bool isSelected)
{
	selected = isSelected;
	if (selected)
	{
		this->initWithFile("Game/building/barrack_selected.png");
		this->setAnchorPoint(Vec2(0.5f, 0.4f));
	}
	else
	{
		this->initWithFile("Game/building/barrack.png");
		this->setAnchorPoint(Vec2(0.5f, 0.4f));
	}
}