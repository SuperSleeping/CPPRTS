#include"GameElement.h"
#include<math.h>
#include"GameInformation.h"

//返回类型GameElement的指针，指向用new建立的一个GameElement对象，便于在不使用的时候用delete释放空间。
GameElement* GameElement::create(const char* filename, Vec2 position,int tm)
{
	
	//创建GameElement中类似Sprite的部分
	GameElement* gameelement = new (std::nothrow)GameElement;
	if (gameelement && gameelement->initWithFile(filename))
	{
		gameelement->team = tm;
		gameelement->setPosition(position);
		//属性设置
		gameelement->positionCurrent = position;
		//自动释放
		gameelement->autorelease();
		return gameelement;
	}
	CC_SAFE_DELETE(gameelement);
	return nullptr;
}

//重载setPosition()
void GameElement::setPosition(const Vec2& pos)
{
	Sprite::setPosition(pos);
	positionCurrent = pos;
	positionCurrentTM = convertToTM(pos);
}