#ifndef _DEVELOPER_H_
#define _DEVELOPER_H_

#include "cocos2d.h"

class Developer : public cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	// a selector callback
	void menuReturn(cocos2d::Ref* pSender);

	// implement the "static create()" method manually
	CREATE_FUNC(Developer);
};

#endif // __DEVELOPER_H__
