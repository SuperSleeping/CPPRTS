#include "Building.h"

Building::Building()
{

}

Building::Building(int type)
{
	//�Զ����뽨����𣬽�����Ӧ���Գ�ʼ��
	buildingType = type;
	
	//С�������ɵ�
	if (buildingType == BuildingType::BARRACK || buildingType == BuildingType::WARFACTORY)
	{
		spawnPoint = Vec2(-48, -24);
	}
	else spawnPoint = Vec2(0, 0);


}

//����setPosition
//				(�Ķ�positionCurrent)
//				(spawnPoint����positionCurrent�䶯��
void Building::setPosition(const Vec2& pos)
{
	GameElement::setPosition(pos);
	spawnPoint += pos;
}

