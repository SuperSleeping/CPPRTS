#ifndef _CHARACTER_H_
#define _CHARACTER_H_

#include "GameElement.h"

class Character : public GameElement
{
public:
	//Create Character
	static Character* create(const char* filename,Vec2 position);

	//�˶����
	Vec2 positionGoal;
	float velocity;
	//���Ŀ��λ���뵱ǰλ�ú�����ƶ�
	void move();

private:
	float distance(Vec2 a, Vec2 b)
	{
		float _distance = sqrt(
			abs(
			(a.x - b.x)*(a.x - b.x) +
				(a.y - b.y)*(a.y - b.y)
			)
		);
		return _distance;
	}
};


#endif