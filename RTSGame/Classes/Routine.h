#ifndef _ROUTINE_H_
#define _ROUTINE_H_

#define _WIDTH_OF_ARRAY_ 118
#define _HEIGHT_OF_ARRAY_ 138

#include "cocos2d.h"
USING_NS_CC;

#include<vector>
#include<math.h>

//////����////// 
#include<iostream>
using std::cin;
using std::cout;
using std::endl;
//////////////// 

using std::vector;

struct Note
{
	//���ڵ�(father��close����еĽǱ�)
	int father;
	//�Լ�
	int x;
	int y;
	//��ʼ�㵽��ǰ��ʵ�ʴ���
	int g;
	//��ǰ�ڵ㵽Ŀ��ڵ����·���Ĺ��ƴ���
	int h;
	//���ƺ�����f = g + h
	int f;

	//���������
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

	//�洢����·����Ҫ�߹���ÿһ��Point
	vector<Point> final_path;

	//������ʼ���յ�����
	void find_a_new_way(Point originTM, Point destinationTM);

private:
	//��ͼ��Ϣ����
	bool (*isBlock)[_HEIGHT_OF_ARRAY_];

	//���Origin
	//�յ�Destination
	Note Origin;
	Note Destination;

	//����
	const int direction[8][2] = 
	{
		//����Ϊ10
		{-1,0},		//West
		{1,0},		//East
		{0,-1},		//North
		{0,1},		//South

		//����Ϊ14
		{1,-1},		//EastNorth
		{-1,1},		//WestSouth
		{-1,-1},	//WestNorth
		{1,1}		//EastSouth
	};

	//������루�����پ��룩
	int distance(Note x)
	{
		int dis = abs(x.x - Destination.x);
		dis += abs(x.y - Destination.y);
		dis = dis * 10;
		return dis;
	}

	//open��洢����������ӽڵ�
	vector<Note> open;
	//close��洢�Ѿ������ȷ�����Ľڵ����Ϣ
	vector<Note> close;
	//�жϸ�λ���Ƿ���Ҫ�����飨δ���=0���ѱ�����open=1���ѱ�����close=2��
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
