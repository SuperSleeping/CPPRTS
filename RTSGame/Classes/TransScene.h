#ifndef _TRANS_SCENE_H_
#define _TRANS_SCNEE_H_

#include"cocos2d.h"
USING_NS_CC;

class TransScene :public Scene
{
public:
	static Scene* createScene();
	virtual bool init();

	//menu
	void newCallback(Ref* pSender);
	void joinCallback(Ref* pSender);
	void returnCallback(Ref* pSender);

	CREATE_FUNC(TransScene);
};
#endif