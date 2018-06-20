#ifndef _ROUTINE_H_
#define _ROUTINE_H_

#include<vector>
#include<math.h>
#include"cocos2d.h"
USING_NS_CC;
using std::vector;

struct Note
{
	//父节点
	Note* father;
	//自己
	int x;
	int y;
	//起始点到当前点实际代价
	float g;
	//当前节点到目标节点最佳路径的估计代价
	float h;
	//估计函数：f = g + h
	float f;

	//运算符重载
	bool operator==(Note note)
	{
		if (x == note.x&&y == note.y)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	bool operator!=(Note note)
	{
		if (x != note.x||y != note.y)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
};



class Routine
{
public:
	Routine(Point origin,Point destination,bool blockMessage[118][138]);
	~Routine();

	//存储最终路径需要走过的每一个Point
	vector<Point> final_path;

private:
	//地图信息置入
	bool (*isBlock)[138];

	//起点Origin
	//终点Destination
	Note Origin;
	Note Destination;

	//点x是否满足每一步的估价要求
	bool meetCommand(Note x);

	//测距：点x到点Destination
	float omin_distance;
	float distance(Note x)
	{
		float f = sqrt((x.x - Destination.x)*(x.x - Destination.x) + (x.y - Destination.y)*(x.y - Destination.y));
		return f;
	}

	//open表存储下一步待测的所有子节点（由close表衍生）
	vector<Note> open;
	//close表存储当前所有符合要求节点的信息
	vector<Note> close;

	void openTableInit();

	void doSearch();
};

#endif