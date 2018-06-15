#include "Basement.h"

Basement::Basement()
	:Building(BASEMENT)
{

}

Basement* Basement::create(Vec2 position)
{
	//创建GameElement中类似Sprite的部分
	Basement* basement = new (std::nothrow)Basement;
	if (basement && basement->initWithFile("Game/building/basement.png"))
	{
		//锚点坐标设置
		basement->setAnchorPoint(Vec2(0.5, 0.3));
		basement->setPosition(position);
		//属性设置
		basement->blood = 200;
		//自动释放
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
		this->setAnchorPoint(Vec2(0.5, 0.3));
	}
	else
	{
		this->initWithFile("Game/building/basement.png");
		this->setAnchorPoint(Vec2(0.5, 0.3));
	}
}