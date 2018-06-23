#ifndef _BARRACK_H_
#define _BARRACK_H_

#include"Building.h"


class Barrack : public Building
{
public:
	static Barrack* create(Vec2 position);

	Barrack();

	virtual void setSelected(bool isSelected);
};

#endif
