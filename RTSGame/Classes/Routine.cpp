#include "Routine.h"


//��֤����origin=destination��ʱ�����routine����·��

Routine::Routine(Point originTM, Point destinationTM, bool blockMessage[118][138])
{
	//������Ϣ
	isBlock = blockMessage;

	Origin.x = (int)originTM.x;
	Origin.y = (int)originTM.y;
	Origin.f = 0;
	Destination.x = (int)destinationTM.x;
	Destination.y = (int)destinationTM.y;

	//Open����ʼ����table����ʼ��
	open.push_back(Origin);
	table[Origin.x][Origin.y] = 1;
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
			if (open.begin != Destination)
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
	for (iter = open.begin() + 1; iter != open.end(); iter++)
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

	//�˸�����
	for (int num = 0; num < 8; num++)
	{
		int x = temp->x + direction[num][0];
		int y = temp->y + direction[num][1];

		//�������Ƿ����ϰ�����Ѿ�������
		if (isBlock[x][y] == 1 || table[x][y] == 1)continue;
		else
		{
			//������ӦNote
			Note note;
			note.x = x;
			note.y = y;
			note.father = &(*(close.end() - 1));
			note.g = temp->f;
			note.h = distance(note);

			//note����Open����
			open.push_back(note);
		}
	}
	
	//ɾ��open�е�temp
	open.erase(temp);

}

