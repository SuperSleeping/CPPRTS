#include"GameElement.h"
#include<math.h>
#include"GameInformation.h"

//��������GameElement��ָ�룬ָ����new������һ��GameElement���󣬱����ڲ�ʹ�õ�ʱ����delete�ͷſռ䡣
GameElement* GameElement::create(const char* filename, Vec2 position,int tm)
{
	
	//����GameElement������Sprite�Ĳ���
	GameElement* gameelement = new (std::nothrow)GameElement;
	if (gameelement && gameelement->initWithFile(filename))
	{
		gameelement->team = tm;
		gameelement->setPosition(position);
		//��������
		gameelement->positionCurrent = position;
		//�Զ��ͷ�
		gameelement->autorelease();
		return gameelement;
	}
	CC_SAFE_DELETE(gameelement);
	return nullptr;
}

//����setPosition()
void GameElement::setPosition(const Vec2& pos)
{
	Sprite::setPosition(pos);
	positionCurrent = pos;
	positionCurrentTM = convertToTM(pos);
}