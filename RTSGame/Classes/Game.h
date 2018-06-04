#ifndef _GAME_H_
#define _GAME_H_

#include "cocos2d.h"

class Game : public cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();
	
	////////////////////////////////
	////////game scene initialize 
	////////////////////////////////
	void sceneLoad();
	void mapInit();


	// a selector callback
	void menuReturn(cocos2d::Ref* pSender);

	// implement the "static create()" method manually
	CREATE_FUNC(Game);
};

#endif // __GAME_H__
