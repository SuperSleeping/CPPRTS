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
	int g;
	//当前节点到目标节点最佳路径的估计代价
	int h;
	//估计函数：f = g + h
	int f;

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

	//方向
	const int direction[8][2] = 
	{
		//距离为10
		{-1,0},		//West
		{1,0},		//East
		{0,-1},		//North
		{0,1},		//South

		//距离为14
		{1,-1},		//EastNorth
		{-1,1},		//WestSouth
		{-1,-1},	//WestNorth
		{1,1}		//EastSouth
	};

	//计算距离（曼哈顿距离）
	int distance(Note x)
	{
		int dis = abs(x.x - Destination.x);
		dis += abs(x.y + Destination.y);
		return dis;
	}
/*	
	float distance(Note x, Note y)
	{
		float f = sqrt((x.x - y.x)*(x.x - y.x) + (x.y - y.y)*(x.y - y.y));
		return f;
	}
*/

	//open表存储待测的所有子节点
	vector<Note> open;
	//close表存储已经检测且确定过的节点的信息
	vector<Note> close;
	//判断该位置是否需要加入检查（检查过=1；未检查=0）
	bool table[118][138];

	void openTableInit();

	void doSearch();
};

#endif