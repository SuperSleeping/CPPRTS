#ifndef _TANK_H_
#define _TANK_H_

#include"Character.h"

class Tank : public Character
{
public:
	static Tank* create(Vec2 position);

};

#endif