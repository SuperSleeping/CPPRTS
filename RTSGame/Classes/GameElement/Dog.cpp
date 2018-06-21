#include "Dog.h"

Dog* Dog::create(Vec2 position)
{
	//����GameElement������Sprite�Ĳ���
	Dog* dog = new (std::nothrow)Dog;
	if (dog && dog->initWithFile("Game/character/dog/leftdown.png"))
	{
		//λ������
		dog->setAnchorPoint(Vec2(0.5, 0));
		dog->setPosition(position);
		dog->positionCurrent = position;
		//��������
		dog->blood = 80;
		dog->bloodNow = 80;
		dog->prize = 150;
		dog->velocity = 25;
		CreateTag++;
		dog->setTag(CreateTag);
		//�Զ��ͷ�
		dog->autorelease();
		return dog;
	}
	CC_SAFE_DELETE(dog);
	return nullptr;
}