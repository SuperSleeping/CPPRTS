#ifndef _WARFACTORY_H_
#define _WARFACTORY_H_

#include "Building.h"
class Warfactory : public Building
{
public:

	static Warfactory* create(Vec2 position);

};

#endif