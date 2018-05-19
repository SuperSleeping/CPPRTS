#ifndef __START_SCENE_H__
#define __START_SCENE_H__

#include "cocos2d.h"

class Start :public cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();

	void menureturnCallback(cocos2d::Ref *pSender);

	CREATE_FUNC(Start);
};

#endif //__START_SCENE_H__