#ifndef _BASEMENT_H_
#define _BASEMENT_H_

#include"Building.h"

class Basement : public Building
{
public:
	Basement();
	static Basement* create(Vec2 position);

	virtual void setSelected(bool isSelected);

};

#endif
