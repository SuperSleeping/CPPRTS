#ifndef _WARFACTORY_H_
#define _WARFACTORY_H_

#include "Building.h"
class Warfactory : public Building
{
public:
	Warfactory();
	static Warfactory* create(Vec2 position);

	virtual void setSelected(bool isSelected);

};

#endif