#ifndef _GAME_STATE_H_
#define _GAME_STATE_H_

class GameState
{
public:
	GameState();
	~GameState();

	//״̬����
	/*
	
	**	Impriority:
	**	selectedState:
	**	0		none
	**	1-6		BuildingSelected
	**	7		CharacterSelected
	
	**
	
	**	Priority:
	**	buildState
	**	0		none
	**	1-6		BuildingSelected
	*/
	int selectedState;
	int buildState;

	//�ı�״̬�Ľӿ�

	void selectedStateChange(int state)
	{
		selectedState = state;
	}
};

#endif

