#ifndef __SETTING_SCENE_H__
#define __SETTING_SCENE_H__

#include "cocos2d.h"

class Setting : public cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();

	void menuokCallback(cocos2d::Ref *pSender);
	void menureturnCallback(cocos2d::Ref *pSender);
	void menusoundCallback(cocos2d::Ref *pSender);
	void menumusicCallback(cocos2d::Ref *pSender);

	CREATE_FUNC(Setting);
};

#endif//__SETTING_SCENE_H__