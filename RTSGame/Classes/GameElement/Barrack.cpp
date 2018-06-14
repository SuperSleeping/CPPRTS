#include "Barrack.h"


Barrack* Barrack::create(Vec2 position)
{
	//创建GameElement中类似Sprite的部分
	Barrack* barrack = new (std::nothrow)Barrack;
	if (barrack && barrack->initWithFile("Game/building/barrack.png"))
	{
		//锚点坐标设置
		barrack->setAnchorPoint(Vec2(0.5, 0.5));
		barrack->setPosition(position);
		//属性设置
		barrack->positionCurrent = position;
		barrack->blood = 200;
		//自动释放
		barrack->autorelease();
		return barrack;
	}
	CC_SAFE_DELETE(barrack);
	return nullptr;
}