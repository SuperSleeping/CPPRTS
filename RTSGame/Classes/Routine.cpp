#include "Routine.h"



Routine::Routine(Point origin, Point destination, bool blockMessage[118][138])
{
	//置入信息
	isBlock = blockMessage;

	Origin.x = (int)origin.x;
	Origin.y = (int)origin.y;
	Destination.x = (int)destination.x;
	Destination.y = (int)destination.y;
	//总长
	omin_distance = distance(Origin);
}


Routine::~Routine()
{
}


void Routine::doSearch()
{
	if (Origin==Destination)
	{
	///	final_path.push_back(Destination);
		return;
	}

	
	vector<Note>::iterator iter;
	for (iter = open.begin(); iter != open.end(); iter++)
	{
		//如果不是终点，推入
		if (*iter != Destination)
		{
			
		}
	}
}

bool Routine::meetCommand(Note x)
{
	//当前距离+估计最短距离<2*总最短距离 即满足要求
	if (x.f <= 2 * omin_distance)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Routine::openTableInit()
{
	
}

