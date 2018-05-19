#ifndef _SETTING_H_
#define _SETTING_H_

#include "cocos2d.h"

class Setting : public cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	// a selector callback
	void menuAudioChange(cocos2d::Ref* pSender);
	void menuReturn(cocos2d::Ref* pSender);

	// implement the "static create()" method manually
	CREATE_FUNC(Setting);
};

#endif // __SETTING_H__
