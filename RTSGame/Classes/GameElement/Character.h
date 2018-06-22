#ifndef _CHARACTER_H_
#define _CHARACTER_H_

#include "GameElement.h"
#include "Routine.h"
#include <vector>
using std::vector;

class Character : public GameElement
{
public:
	Character();
	~Character();

	//Create Character
	static Character* create(const char* filename,Vec2 position);

	//��������
	enum CharacterType
	{
		INFANTRY = 6,
		DOG,
		TANK,
		CharacterChosen
	};

	//ѡ�б��
	Sprite* shadow;
	virtual void setSelected(bool isSelected);

	//λ��
	Vec2 positionGoal;
	Vec2 positionGoalTM;
	bool positionGoal_change = 0;
	void setPositionGoal(Point positionWorld)
	{
		positionGoal = positionWorld;
		positionGoalTM = convertToTiledMap(positionWorld);
		positionGoal_change = true;
	}

	//�˶����
	float velocity;
	DrawNode* drawRoutineNote;

	//�����ƶ�
	void move();
	Routine* routine;
	void drawRoutine();

private:
	float distance(Vec2 a, Vec2 b)
	{
		float _distance = sqrt((a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y));
		return _distance;
	}
};


#endif