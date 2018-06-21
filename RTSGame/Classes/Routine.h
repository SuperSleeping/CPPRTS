#ifndef _ROUTINE_H_
#define _ROUTINE_H_

#include<vector>
#include<math.h>
#include"cocos2d.h"
USING_NS_CC;
using std::vector;

struct Note
{
	//���ڵ�
	Note* father;
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

	//�洢����·����Ҫ�߹���ÿһ��Point
	vector<Point> final_path;

private:
	//��ͼ��Ϣ����
	bool (*isBlock)[138];

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

	//open��洢����������ӽڵ�
	vector<Note> open;
	//close��洢�Ѿ������ȷ�����Ľڵ����Ϣ
	vector<Note> close;
	//�жϸ�λ���Ƿ���Ҫ�����飨����=1��δ���=0��
	bool table[118][138];

	void openTableInit();

	void doSearch();
};

#endif