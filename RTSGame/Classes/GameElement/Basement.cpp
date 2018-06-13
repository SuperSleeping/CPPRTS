#include "Basement.h"

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
		basement->positionCurrent = position;
		basement->blood = 200;
		//自动释放
		basement->autorelease();
		return basement;
	}
	CC_SAFE_DELETE(basement);
	return nullptr;
}