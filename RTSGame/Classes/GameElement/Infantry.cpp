#include "Infantry.h"

Infantry* Infantry::create(const char* filename, Vec2 position)
{
	//创建GameElement中类似Sprite的部分
	Infantry* infantry = new (std::nothrow)Infantry;
	if (infantry && infantry->initWithFile(filename))
	{
		infantry->setPosition(position);
		//属性设置
		infantry->positionCurrent = position;

		//自动释放
		infantry->autorelease();
		return infantry;
	}
	CC_SAFE_DELETE(infantry);
}