#ifndef _BUILDING_H_
#define _BUILDING_H_

#include"GameElement.h"

class Building : public GameElement
{
public:
	enum BuildingType
	{
		BASEMENT=1,
		POWERPLANT,
		MINEFIELD,
		BARRACK,
		WARFACTORY
	};
};

#endif