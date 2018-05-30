#ifndef _MY_SETTING_SCENE_
#define _MY_SETTING_SCENE_

#include "cocos2d.h"

USING_NS_CC;

class mysetting :public Scene
{
public:
	static Scene* createScene();
	virtual bool init();

	//menu
	void difficultyCallback(Ref* pSender);
	void speedCallback(Ref* pSender);
	void soundCallback(Ref* pSender);
	void backtorootCallback(Ref* pSender);

	CREATE_FUNC(mysetting);
};
#endif