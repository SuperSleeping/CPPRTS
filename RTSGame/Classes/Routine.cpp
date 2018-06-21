#include "Routine.h"


//��֤����origin=destination��ʱ�����routine����·��

Routine::Routine(bool blockMessage[118][138])
{
	//������Ϣ
	isBlock = blockMessage;
}

void Routine::FromStartToEnd(Point originTM, Point destinationTM)
{
	Origin.x = (int)originTM.x;
	Origin.y = (int)originTM.y;
	Origin.f = 0;
	Destination.x = (int)destinationTM.x;
	Destination.y = (int)destinationTM.y;

	//Open����ʼ����table����ʼ��
	open.push_back(Origin);
	table[Origin.x][Origin.y] = 1;

	//final_path����
	doSearch();
}

Routine::~Routine()
{
}


void Routine::doSearch()
{
	vector<Note>::iterator GOAL;
	while (true)
	{
		//����Open���
		openTableInit();

		//�����Ƿ��ܹ���һ������·��
		GOAL = open.begin();
		vector<Note>::iterator iter;
		for (iter = open.begin(); iter != open.end(); iter++)
		{
			//������յ�
			if (*iter == Destination)
			{
				if (GOAL->f > iter->f)
				{
					GOAL = iter;
				}
			}
		}

		//���û������·��������search���̣������ҳ�����·��
		if (GOAL == open.begin())
		{
			//open.begin()����Destination
			if (*open.begin() != Destination)
				continue;
			//�ҵ�Destination��
			else
			{
				break;
			}
		}
		//�ҵ�Destination��
		else
		{
			break;
		}
	}

	//�ҵ�����·��������·����������final_path
	Note* Goal = &(*GOAL);
	while (*Goal != Origin)
	{
		float x, y;
		x = Goal->x;
		y = Goal->y;
		final_path.push_back(Vec2(x, y));
		//���ݵ����ڵ�
		Goal = Goal->father;
	}
}

void Routine::openTableInit()
{
	//�ҵ�open����f��С��Note������temp
	vector<Note>::iterator temp = open.begin();
	vector<Note>::iterator iter;
	for (iter = open.begin(); iter != open.end(); iter++)
	{
		//����open����ҵ����ƴ�����С��Note
		if (temp->f > iter->f)
		{
			temp = iter;
		}
	}

	//temp����Close����У��ı�teble
	close.push_back(*temp);
	table[temp->x][temp->y] = 1;

	//����temp��Ϣ��ɾ��open�е�temp
	int X, Y, G;
	X = temp->x;
	Y = temp->y;
	G = temp->g;

	open.erase(temp);

	//�˸�����
	for (int num = 0; num < 8; num++)
	{
		int dis;
		if (num < 4)
		{
			dis = 10;
		}
		else
		{
			dis = 14;
		}
		int x = X + direction[num][0];
		int y = Y + direction[num][1];

		//�������Ƿ����ϰ�����Ѿ�������
		if (isBlock[x][y] == 1 || table[x][y] == 1)continue;
		else
		{
			//������ӦNote
			Note note;
			note.x = x;
			note.y = y;
			note.father = &(*(close.end() - 1));
			note.g = G + dis;
			note.h = distance(note);
			note.f = note.g + note.h;

			//note����Open����
			open.push_back(note);
		}
	}
}

