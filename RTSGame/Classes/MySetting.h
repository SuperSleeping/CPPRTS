#ifndef _MY_SETTING_SCENE_
#define _MY_SETTING_SCENE_

#include "cocos2d.h"
#include "UserDefault.h"
#include "SimpleAudioEngine.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

class MySetting:public Scene
{
public:
	static Scene* createScene();
	virtual bool init();

	//menu
	void musicCallback(Ref* pSender);
	void soundCallback(Ref* pSender);
	void returnCallback(Ref* pSender);
	void saveCallback(Ref* pSender);

	CREATE_FUNC(MySetting);
};
#endif