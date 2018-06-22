#include "Infantry.h"

Infantry* Infantry::create(Vec2 position)
{
	//创建GameElement中类似Sprite的部分
	Infantry* infantry = new (std::nothrow)Infantry;
	if (infantry && infantry->initWithFile("Game/character/infantry/leftdown.png"))
	{
		//图片设置
		infantry->setAnchorPoint(Vec2(0.5, 0));
		infantry->setPosition(position);
		//初始位置设置
		infantry->setPositionCurrent(position);
		infantry->setPositionGoal(position);
		//属性设置
		infantry->blood = 125;
		infantry->prize = 120;
		infantry->velocity = 150;
		//自动释放
		infantry->autorelease();
		return infantry;
	}
	CC_SAFE_DELETE(infantry);
	return nullptr;
}