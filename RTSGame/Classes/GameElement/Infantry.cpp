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

		//�Զ��ͷ�
		infantry->autorelease();
		return infantry;
	}
	CC_SAFE_DELETE(infantry);
}