#ifndef _POWERPLANT_H_
#define _POWERPLANT_H_

#include "Building.h"
class Powerplant : public Building
{
public:
	Powerplant();

	static Powerplant* create(Vec2 position);

	virtual void setSelected(bool isSelected);

};

#endif