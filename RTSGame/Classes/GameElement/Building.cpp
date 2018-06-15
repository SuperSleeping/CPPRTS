#include "Building.h"

Building::Building()
{

}

Building::Building(int type)
{
	//�Զ����뽨����𣬽�����Ӧ���Գ�ʼ��
	buildingType = type;
	
	//С�������ɵ�
	spawnPoint = positionCurrent + 2 * Vec2(48, -24);



}

//����setPosition
//				(�Ķ�positionCurrent)
//				(spawnPoint����positionCurrent�䶯��
void Building::setPosition(const Vec2& pos)
{
	Sprite::setPosition(pos);
	positionCurrent = pos;
	positionCurrentTM = convertToTM(pos);
	spawnPoint += pos;
}

