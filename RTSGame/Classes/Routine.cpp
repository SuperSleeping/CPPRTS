#include "Routine.h"


//保证不在origin=destination的时候调用routine搜索路径

Routine::Routine(Point originTM, Point destinationTM, bool blockMessage[118][138])
{
	//置入信息
	isBlock = blockMessage;

	Origin.x = (int)originTM.x;
	Origin.y = (int)originTM.y;
	Origin.f = 0;
	Destination.x = (int)destinationTM.x;
	Destination.y = (int)destinationTM.y;

	//Open表格初始化，table表格初始化
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
		//更新Open表格
		openTableInit();

		//查找是否能构成一条完整路径
		GOAL = open.begin();
		vector<Note>::iterator iter;
		for (iter = open.begin(); iter != open.end(); iter++)
		{
			//如果有终点
			if (*iter == Destination)
			{
				if (GOAL->f > iter->f)
				{
					GOAL = iter;
				}
			}
		}

		//如果没有完整路径，继续search过程，回溯找出完整路径
		if (GOAL == open.begin())
		{
			//open.begin()不是Destination
			if (open.begin != Destination)
				continue;
			//找到Destination了
			else
			{
				break;
			}
		}
		//找到Destination了
		else
		{
			break;
		}
	}

	//找到完整路径，回溯路径倒序填入final_path
	Note* Goal = &(*GOAL);
	while (*Goal != Origin)
	{
		float x, y;
		x = Goal->x;
		y = Goal->y;
		final_path.push_back(Vec2(x, y));
		//回溯到父节点
		Goal = Goal->father;
	}
}

void Routine::openTableInit()
{
	//找到open表中f最小的Note，命名temp
	vector<Note>::iterator temp = open.begin();
	vector<Note>::iterator iter;
	for (iter = open.begin() + 1; iter != open.end(); iter++)
	{
		//遍历open表格找到估计代价最小的Note
		if (temp->f > iter->f)
		{
			temp = iter;
		}
	}

	//temp加入Close表格中，改变teble
	close.push_back(*temp);
	table[temp->x][temp->y] = 1;

	//八个方向
	for (int num = 0; num < 8; num++)
	{
		int x = temp->x + direction[num][0];
		int y = temp->y + direction[num][1];

		//检查格子是否有障碍物或已经加入检查
		if (isBlock[x][y] == 1 || table[x][y] == 1)continue;
		else
		{
			//创建相应Note
			Note note;
			note.x = x;
			note.y = y;
			note.father = &(*(close.end() - 1));
			note.g = temp->f;
			note.h = distance(note);

			//note加入Open队列
			open.push_back(note);
		}
	}
	
	//删除open中的temp
	open.erase(temp);

}

