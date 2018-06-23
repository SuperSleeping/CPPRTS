#ifndef _DOG_H_
#define _DOG_H_

#include "Character.h"

class Dog : public Character
{
public:
	static Dog* create(Vec2 position);
};


#endif