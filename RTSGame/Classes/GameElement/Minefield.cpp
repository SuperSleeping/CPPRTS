#include "Minefield.h"
#include"GameInformation.h"
Minefield::Minefield()
	:Building(MINEFIELD)
{

}

Minefield* Minefield::create(Vec2 position)
{
	//创建GameElement中类似Sprite的部分
	Minefield* minefield = new (std::nothrow)Minefield;
	if (minefield && minefield->initWithFile("Game/building/minefield.png"))
	{
		//锚点坐标设置
		minefield->setAnchorPoint(Vec2(0.5, 0.25));
		minefield->setPosition(position);
		//属性设置
		minefield->positionCurrent = position;
		minefield->blood = 200;
		minefield->bloodNow = 200;
		CreateTag++;
		minefield->setTag(CreateTag);
		//自动释放
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