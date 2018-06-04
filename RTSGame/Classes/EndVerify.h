#ifndef _END_VERIFY_SCENE_H_
#define _END_VERIFY_SCNEE_H_

#include"cocos2d.h"
USING_NS_CC;

class EndVerify :public Scene
{
public:
	static Scene* createScene();
	virtual bool init();

	//menu
	void verifyCallback(Ref* pSender);
	void cancelCallback(Ref* pSender);

	CREATE_FUNC(EndVerify);
};
#endif