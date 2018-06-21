#ifndef _CHARACTER_H_
#define _CHARACTER_H_

#include "GameElement.h"

extern int MapInfo[118][138];
extern int Block[118][138];
extern int Characters[118][138];
extern int Buildings[118][138];

extern Point convertToTiledMap(Point pt);
extern Point convertFromTMToWorld(Point pt);


class Character : public GameElement
{
public:
	Character();

	//Create Character
	static Character* create(const char* filename, Vec2 position);

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
	Vec2 positionNow;
	Vec2 positionTarget;
	float velocity;
	//检测目标位置与当前位置后进行移动
	void move();

	int repeat = 0;
	void updateMove(float di);
	/*void updateMove(float di) {
		for (int i = 0; i < 118; i++)
		{
			for (int j = 0; j < 138; j++)
			{
				Characters[i][j] = 0;
			}
		}
		for (Infantry* infa : infantryGroup[myTeam])
		{
			int x = infa->positionNow.x;
			int y = infa->positionNow.y;
			Characters[x][y] -= 700;
		}
		Point TMposition = convertToTiledMap(this->getPosition());
		positionNow = TMposition;
		int MapCondition[118][138];
		if (TMposition != positionGoal)
		{
			if (!this->numberOfRunningActions())
			{
				for (int i = 0; i < 118; i++)
				{
					for (int j = 0; j < 138; j++)
					{
						MapCondition[i][j] = MapInfo[i][j] + Block[i][j] + Buildings[i][j] + Characters[i][j] + this->MapDestination[i][j];
					}
				}
				Point BestTarget = TMposition;
				int x = TMposition.x;
				int y = TMposition.y;
				int direction = -1;
				int best = MapCondition[x][y];
				for (int i = 0; i < 8; i++)
				{
					if (MapCondition[x + DIRECTION[i][0]][y + DIRECTION[i][1]] > best)
					{
						best = MapCondition[x + DIRECTION[i][0]][y + DIRECTION[i][1]];
						direction = i;
					}
				}
				if (direction == -1)
				{
					positionGoal = TMposition;
				}
				else
				{
					BestTarget.x += DIRECTION[direction][0];
					BestTarget.y += DIRECTION[direction][1];
					MoveTo* move = MoveTo::create(0.5f, convertFromTMToWorld(BestTarget));
					this->runAction(move);
				}

			}
		}
	}*/

	int MapDestination[118][138];

	void setMapDestination(Point pt)
	{
		int x = pt.x;
		int y = pt.y;
		int q = 5;					//权重值

		for (int i = 0; i < 118; i++)
		{
			for (int j = 0; j < 138; j++)
			{
				MapDestination[i][j] = 0;
			}
		}

		MapDestination[x][y] = 700;				//确保所有辐射值都为正数
		for (int i = x; i >= 0; i--)
		{
			MapDestination[i][y] = 700 - q * (x - i);
			for (int j = y - 1; j >= 0; j--)
			{
				MapDestination[i][j] = MapDestination[i][y] - q * (y - j);
			}
			for (int j = y + 1; j < 138; j++)
			{
				MapDestination[i][j] = MapDestination[i][y] - q * (j - y);
			}
		}
		for (int i = x; i < 118; i++)
		{
			MapDestination[i][y] = 700 - q * (i - x);
			for (int j = y - 1; j >= 0; j--)
			{
				MapDestination[i][j] = MapDestination[i][y] - q * (y - j);
			}
			for (int j = y + 1; j < 138; j++)
			{
				MapDestination[i][j] = MapDestination[i][y] - q * (j - y);
			}
		}
	}

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