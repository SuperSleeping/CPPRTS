#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

class HelloWorld : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

	virtual void update(float dt);
    
    // a selector callback
	void menuIntoLabelNewgame(cocos2d::Ref* pSender);
	void menuIntoLabelSetting(cocos2d::Ref* pSender);
	void menuIntoLabelDeveloper(cocos2d::Ref* pSender);
 
	void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
};

#endif // __HELLOWORLD_SCENE_H__
