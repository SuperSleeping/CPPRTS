#include "Powerplant.h"

Powerplant::Powerplant()
	:Building(POWERPLANT)
{

}

Powerplant* Powerplant::create(Vec2 position)
{
	//����GameElement������Sprite�Ĳ���
	Powerplant* powerplant = new (std::nothrow)Powerplant;
	if (powerplant && powerplant->initWithFile("Game/building/powerplant.png"))
	{
		//ê����������
		powerplant->setAnchorPoint(Vec2(0.5, 0.3));
		powerplant->setPosition(position);
		//��������
		powerplant->selected = 0;
		powerplant->positionCurrent = position;
		powerplant->blood = 200;
		//�Զ��ͷ�
		powerplant->autorelease();
		return powerplant;
	}
	CC_SAFE_DELETE(powerplant);
	return nullptr;
}