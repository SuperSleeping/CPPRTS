#include "Routine.h"



Routine::Routine(Point origin, Point destination, bool blockMessage[118][138])
{
	//������Ϣ
	isBlock = blockMessage;

	Origin.x = (int)origin.x;
	Origin.y = (int)origin.y;
	Destination.x = (int)destination.x;
	Destination.y = (int)destination.y;
	//�ܳ�
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
		//��������յ㣬����
		if (*iter != Destination)
		{
			
		}
	}
}

bool Routine::meetCommand(Note x)
{
	//��ǰ����+������̾���<2*����̾��� ������Ҫ��
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

