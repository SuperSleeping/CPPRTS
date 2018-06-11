#ifndef _ROOT_SCENE_H_
#define _ROOT_SCNEE_H_

#include"cocos2d.h"
#include "SimpleAudioEngine.h"
#include "UserDefault.h"
USING_NS_CC;

class RootScene:public Scene
{
public:
	static Scene* createScene();
	virtual bool init();
	virtual void onEnter();
	virtual void onEnterTransitionDidFinish();
	virtual void onExit();
	virtual void onExitTransitionDidStart();
	virtual void cleanup();

	//menu
	void menusinmemchoiceCallback(Ref* pSender);
	void menumulmemchoiceCallback(Ref* pSender);
	void menusettingCallback(Ref* pSender);
	void menumakerCallback(Ref* pSender);
	void menuendCallback(Ref* pSender);

	CREATE_FUNC(RootScene);
};
#endif