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
	float g;
	//��ǰ�ڵ㵽Ŀ��ڵ����·���Ĺ��ƴ���
	float h;
	//���ƺ�����f = g + h
	float f;

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

	//��x�Ƿ�����ÿһ���Ĺ���Ҫ��
	bool meetCommand(Note x);

	//��ࣺ��x����Destination
	float omin_distance;
	float distance(Note x)
	{
		float f = sqrt((x.x - Destination.x)*(x.x - Destination.x) + (x.y - Destination.y)*(x.y - Destination.y));
		return f;
	}

	//open��洢��һ������������ӽڵ㣨��close��������
	vector<Note> open;
	//close��洢��ǰ���з���Ҫ��ڵ����Ϣ
	vector<Note> close;

	void openTableInit();

	void doSearch();
};

#endif