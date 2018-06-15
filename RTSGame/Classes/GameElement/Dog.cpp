#include "Dog.h"

Dog* Dog::create(Vec2 position)
{
	//创建GameElement中类似Sprite的部分
	Dog* dog = new (std::nothrow)Dog;
	if (dog && dog->initWithFile("Game/character/dog/leftdown.png"))
	{
		//位置设置
		dog->setAnchorPoint(Vec2(0.5, 0));
		dog->setPosition(position);
		dog->positionCurrent = position;
		//属性设置
		dog->blood = 80;
		dog->prize = 150;
		dog->velocity = 25;
		//自动释放
		dog->autorelease();
		return dog;
	}
	CC_SAFE_DELETE(dog);
	return nullptr;
}