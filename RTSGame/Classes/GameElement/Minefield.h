#ifndef _MINEFIELD_H_
#define _MINEFIELD_H_

#include "Building.h"
class Minefield : public Building
{
public:
	
	Minefield();
	static Minefield* create(Vec2 position);

};

#endif