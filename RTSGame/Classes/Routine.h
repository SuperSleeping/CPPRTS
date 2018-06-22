#ifndef _ROUTINE_H_
#define _ROUTINE_H_

#define _WIDTH_OF_ARRAY_ 118
#define _HEIGHT_OF_ARRAY_ 138

#include "cocos2d.h"
USING_NS_CC;

#include<vector>
#include<math.h>

//////调试////// 
#include<iostream>
using std::cin;
using std::cout;
using std::endl;
//////////////// 

using std::vector;

struct Note
{
	//父节点(father是close表格中的角标)
	int father;
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
		if (this == NULL || &note == NULL)
			return false;
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
		if (this == NULL || &note == NULL)
			return false;
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
	Routine(bool blockMessage[_WIDTH_OF_ARRAY_][_HEIGHT_OF_ARRAY_]);
	~Routine();

	//存储最终路径需要走过的每一个Point
	vector<Point> final_path;

	//导入起始、终点坐标
	void find_a_new_way(Point originTM, Point destinationTM);

private:
	//地图信息置入
	bool (*isBlock)[_HEIGHT_OF_ARRAY_];

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
		dis += abs(x.y - Destination.y);
		dis = dis * 10;
		return dis;
	}

	//open表存储待测的所有子节点
	vector<Note> open;
	//close表存储已经检测且确定过的节点的信息
	vector<Note> close;
	//判断该位置是否需要加入检查（未检查=0；已被加入open=1；已被加入close=2）
	int table[118][138] = { 0 };
	void table_clear()
	{
		for (int i = 0; i < _WIDTH_OF_ARRAY_; i++)
		{
			for (int j = 0; j < _HEIGHT_OF_ARRAY_; j++)
			{
				table[i][j] = 0;
			}
		}
	}

	Note Search();

	void findPath();
};

#endif
