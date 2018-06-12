#ifndef _INFANTRY_H_
#define _INFANTRY_H_

#include "Character.h"

class Infantry : public Character
{
public:
	//Create Character
	static Infantry* create(const char* filename, Vec2 position);

	//״̬
	int blood;

};

#endif