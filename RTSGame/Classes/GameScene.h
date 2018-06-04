#ifndef _GAME_SCENE_
#define _GAME_SCENE_

#include "cocos2d.h"

USING_NS_CC;

class GameScene :public Scene
{
public:
	static Scene* createScene();
	virtual bool init();

	CREATE_FUNC(GameScene);
};
#endif