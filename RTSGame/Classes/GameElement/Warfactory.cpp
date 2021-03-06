#include "Warfactory.h"
#include"GameInformation.h"
Warfactory::Warfactory()
	:Building(WARFACTORY)
{

}

Warfactory* Warfactory::create(Vec2 position)
{
	//创建GameElement中类似Sprite的部分
	Warfactory* warfactory = new (std::nothrow)Warfactory;
	if (warfactory && warfactory->initWithFile("Game/building/warfactory.png"))
	{
		//锚点坐标设置
		warfactory->setAnchorPoint(Vec2(0.5f, 0.4f));
		warfactory->setPosition(position);
		//属性设置
		warfactory->blood = 2000;
		warfactory->bloodNow = 2000;
		CreateTag++;
		warfactory->setTag(CreateTag);
		//自动释放
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
		this->setAnchorPoint(Vec2(0.5f, 0.4f));
	}
	else
	{
		this->initWithFile("Game/building/warfactory.png");
		this->setAnchorPoint(Vec2(0.5f, 0.4f));
	}
}

