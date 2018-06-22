#include "Tank.h"

Tank* Tank::create(Vec2 position)
{
	//创建GameElement中类似Sprite的部分
	Tank* tank = new (std::nothrow)Tank;
	if (tank && tank->initWithFile("Game/character/tank/leftdown.png"))
	{
		//位置设置
		tank->setAnchorPoint(Vec2(0.5, 0));
		tank->setPosition(position);
		//初始位置设置
		tank->setPositionCurrent(position);
		tank->setPositionGoal(position);
		//属性设置
		tank->blood = 170;
		tank->prize = 250;
		tank->velocity = 15;
		//自动释放
		tank->autorelease();
		return tank;
	}
	CC_SAFE_DELETE(tank);
	return nullptr;
}