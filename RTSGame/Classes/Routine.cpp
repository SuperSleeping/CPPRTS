#include "Routine.h"


//保证不在origin=destination的时候调用routine搜索路径

Routine::Routine(bool blockMessage[_WIDTH_OF_ARRAY_][_HEIGHT_OF_ARRAY_])
{
	//置入信息
	isBlock = blockMessage;
}

void Routine::find_a_new_way(Point originTM, Point destinationTM)
{
	Origin.x = (int)originTM.x;
	Origin.y = (int)originTM.y;
	Origin.f = 0;
	Destination.x = (int)destinationTM.x;
	Destination.y = (int)destinationTM.y;

	//Open/Close表格初始化，table表格初始化
	open.clear();
	close.clear();
	table_clear();
	open.push_back(Origin);
	table[Origin.x][Origin.y] = 1;

	//final_path更新
	findPath();

	log("pathFound");
}

Routine::~Routine()
{
}


void Routine::findPath()
{
	vector<Note>::iterator GOAL;

	//更新Open表格
	Note Goal = Search();

	//找到完整路径，回溯路径倒序填入final_path
	while (Goal != Origin)
	{
		float x, y;
		x = Goal.x;
		y = Goal.y;
		final_path.push_back(Vec2(x, y));
		//回溯到父节点
		Goal = close[Goal.father];
	}
}

Note Routine::Search()
{
	while (true)
	{
		//找到open表中f最小的Note，命名*temp
		vector<Note>::iterator temp = open.begin();
		vector<Note>::iterator iter;
		for (iter = open.begin(); iter != open.end(); iter++)
		{
			//遍历open表格找到估计代价最小的Note
			if (temp->f > iter->f)
			{
				temp = iter;
			}
		}

		//open表已空，说明没有路径
		if (open.size() == 0)
		{
			return Origin;
		}
		//或者如果目前f最小的Note就是Destination，则返回该点且可以进行回溯路径的工作
		else if (*temp == Destination)
		{
			return *temp;
		}

		//temp加入Close表格中，改变table
		close.push_back(*temp);
		table[temp->x][temp->y] = 2;

		//保存temp信息后删除open中的temp
		int X, Y, G;
		X = temp->x;
		Y = temp->y;
		G = temp->g;

		open.erase(temp);

		//八个方向
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

			//检查格子是否在范围内
			if (x<0 || x>_WIDTH_OF_ARRAY_ || y<0 || y>_HEIGHT_OF_ARRAY_)
			{
				continue;
			}

			//检查格子是否有障碍物
			//或格子是否已加入close队列
			if (isBlock[x][y] == 1 || table[x][y] == 2)
			{
				continue;
			}
			//检查格子是否已加入open队列
			else if (table[x][y] == 1)
			{
				//寻找对应的Note
				Note temp;
				temp.x = x;
				temp.y = y;
				for (Note search : open)
				{
					if (temp == search)
					{
						//通过当前节点有更小的起点代价G
						if (search.g > G + dis)
						{
							//将当前节点设置成父节点并且改变g值
							search.father = close.size() - 1;
							search.g = G + dis;
							search.f = search.g + search.h;
						}
					}
				}
			}
			else
			{
				//创建相应Note
				Note note;
				note.x = x;
				note.y = y;
				note.father = close.size() - 1;
				note.g = G + dis;
				note.h = distance(note);
				note.f = note.g + note.h;

				//note加入Open队列
				open.push_back(note);
				table[x][y] = 1;
			}
		}
	}
}

