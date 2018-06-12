#ifndef _ABOUT_SCENE_H_
#define _ABOUT_SCNEE_H_

#include"cocos2d.h"
USING_NS_CC;

class AboutScene :public Scene
{
public:
	static Scene* createScene();
	virtual bool init();

	//menu
	void returnCallback(Ref* pSender);

	CREATE_FUNC(AboutScene);
};
#endif