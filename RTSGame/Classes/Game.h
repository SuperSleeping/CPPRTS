#ifndef _GAME_H_
#define _GAME_H_


#include "cocos2d.h"

USING_NS_CC;

class Game : public cocos2d::Scene
{
private:
	Vec2 visibleSize;
	
	TMXTiledMap* map;
	CCTMXLayer* _background;
	CCTMXLayer* _object;
	CCTMXLayer* _meta;

	Sprite *player;

	int mapType;
	void setMapType1(cocos2d::Ref* pSender);
	void setMapType2(cocos2d::Ref* pSender);


public:
	static cocos2d::Scene* createScene();


	virtual bool init();

	////////////////////////////////
	////////game scene initialize 
	////////////////////////////////

	//map initialize
	void mapChoice();
	void mapInit(int type);

	//event
	EventDispatcher* dispatcher;

	virtual void keyPressed(EventKeyboard::KeyCode keycode, cocos2d::Event *event);


	// a selector callback
	void menuReturn(cocos2d::Ref* pSender);

	//
	virtual void update(float dt);

	// implement the "static create()" method manually
	CREATE_FUNC(Game);
};

#endif // __GAME_H__
