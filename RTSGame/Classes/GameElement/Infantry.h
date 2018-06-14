#ifndef _INFANTRY_H_
#define _INFANTRY_H_

#include "Character.h"

class Infantry : public Character
{
public:
	//Create Character
	static Infantry* create(Vec2 position);


};

#endif