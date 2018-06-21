#ifndef _CHARACTER_H_
#define _CHARACTER_H_

#include "GameElement.h"
#include <vector>
using std::vector;

class Character : public GameElement
{
public:
	Character();

	//Create Character
	static Character* create(const char* filename,Vec2 position);

	//人物类型
	enum CharacterType
	{
		INFANTRY = 6,
		DOG,
		TANK,
		CharacterChosen
	};

	//选中标记
	Sprite* shadow;
	virtual void setSelected(bool isSelected);

	//运动相关
	Vec2 positionGoal;
	float velocity;
	DrawNode* drawRoutine;

	//进行移动
	void move();
	vector<Point> path;
	void pathInit(vector<Point> &final_path);

private:
	float distance(Vec2 a, Vec2 b)
	{
		float _distance = sqrt((a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y));
		return _distance;
	}
};


#endif