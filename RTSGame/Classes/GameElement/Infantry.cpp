#include "Infantry.h"

Infantry* Infantry::create(const char* filename, Vec2 position)
{
	//����GameElement������Sprite�Ĳ���
	Infantry* infantry = new (std::nothrow)Infantry;
	if (infantry && infantry->initWithFile(filename))
	{
		infantry->setPosition(position);
		//��������
		infantry->positionCurrent = position;
		infantry->velocity = 10;
		//�Զ��ͷ�
		infantry->autorelease();
		return infantry;
	}
	CC_SAFE_DELETE(infantry);
	return nullptr;
}