#include "Warfactory.h"

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
		warfactory->setAnchorPoint(Vec2(0.5, 0.4));
		warfactory->setPosition(position);
		//属性设置
		warfactory->blood = 200;
		//自动释放
		warfactory->autorelease();
		return warfactory;
	}
	CC_SAFE_DELETE(warfactory);
	return nullptr;
}
