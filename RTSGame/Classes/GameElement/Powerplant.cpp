#include "Powerplant.h"

Powerplant::Powerplant()
	:Building(POWERPLANT)
{

}

Powerplant* Powerplant::create(Vec2 position)
{
	//创建GameElement中类似Sprite的部分
	Powerplant* powerplant = new (std::nothrow)Powerplant;
	if (powerplant && powerplant->initWithFile("Game/building/powerplant.png"))
	{
		//锚点坐标设置
		powerplant->setAnchorPoint(Vec2(0.5, 0.3));
		powerplant->setPosition(position);
		//属性设置
		powerplant->selected = 0;
		powerplant->positionCurrent = position;
		powerplant->blood = 200;
		//自动释放
		powerplant->autorelease();
		return powerplant;
	}
	CC_SAFE_DELETE(powerplant);
	return nullptr;
}