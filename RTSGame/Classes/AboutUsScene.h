#ifndef __ABOUTUS_SCENE_H__
#define __ABOUTUS_SCENE_H__

#include "cocos2d.h"

class AboutUs :public cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();

	void menureturnCallback(cocos2d::Ref *pSender);

	CREATE_FUNC(AboutUs);
};

#endif //__ABOUTUS_SCENE_H__