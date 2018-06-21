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
	findPath();
}

Routine::~Routine()
{
}


void Routine::findPath()
{
	vector<Note>::iterator GOAL;

	//����Open���
	Note Goal = Search();

	//�ҵ�����·��������·����������final_path
	while (Goal != Origin)
	{
		float x, y;
		x = Goal.x;
		y = Goal.y;
		final_path.push_back(Vec2(x, y));
		//���ݵ����ڵ�
		Goal = close[Goal.father];
	}
}

Note Routine::Search()
{
	while (true)
	{
		//�ҵ�open����f��С��Note������*temp
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

		//���Ŀǰf��С��Note����Destination���򷵻ظõ��ҿ��Խ��л���·���Ĺ���
		if (*temp == Destination)
		{
			return *temp;
		}


		//temp����Close����У��ı�table
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

			//�������Ƿ����ϰ���
			if (isBlock[x][y] == 1)
			{
				continue;
			}
			//�������Ƿ��Ѽ���open����
			else if (table[x][y] == 1)
			{
				//Ѱ�Ҷ�Ӧ��Note
				Note temp;
				temp.x = x;
				temp.y = y;
				for (Note search : open)
				{
					if (temp == search)
					{
						//ͨ����ǰ�ڵ��и�С��������G
						if (search.g > G + dis)
						{
							//����ǰ�ڵ����óɸ��ڵ㲢�Ҹı�gֵ
							search.father = close.size() - 1;
							search.g = G + dis;
						}
					}
				}
			}
			else
			{
				//������ӦNote
				Note note;
				note.x = x;
				note.y = y;
				note.father = close.size() - 1;
				note.g = G + dis;
				note.h = distance(note);
				note.f = note.g + note.h;

				//note����Open����
				open.push_back(note);

				log("OpenSize=");
				printf("%d", open.size());
			}
		}
	}
}

