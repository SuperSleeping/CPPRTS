#ifndef _GAME_SCENE_
#define _GAME_SCENE_

#include "cocos2d.h"

USING_NS_CC;

class GameScene :public Scene
{
public:
	static Scene* createScene();
	virtual bool init();
	//static cocos2d::Scene* createScene();

	//virtual void update(float di) override;
	void buildRespone(const std::string &data);
	void createRespone(const std::string &data);
	void attackRespone(const std::string &data);
	void moveRespone(const std::string &data);
	void updateErase(float di);
	CREATE_FUNC(GameScene);
};
#endif
