#ifndef _GAME_STATE_H_
#define _GAME_STATE_H_

class GameState
{
public:
	GameState();
	~GameState();

	//状态参数
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

	//改变状态的接口

	void selectedStateChange(int state)
	{
		selectedState = state;
	}
};

#endif

