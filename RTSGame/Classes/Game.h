#ifndef _GAME_H_
#define _GAME_H_


#include "cocos2d.h"
#include <vector>

#include "GameElement/GameElement.h"

USING_NS_CC;
using std::vector;

class Game : public cocos2d::Scene
{
private:
	Vec2 visibleSize;
	
	TMXTiledMap* map;
	TMXLayer* _background;
	TMXLayer* _object;
	TMXLayer* _meta;

	Sprite *player;

	int mapType;
//	void setMapType1(cocos2d::Ref* pSender);
//	void setMapType2(cocos2d::Ref* pSender);

	/**********************************************/
	/*Set Up Vectors To Store Object (GameElement)*/
	/**********************************************/

	std::vector<GameElement*> character;
	std::vector<GameElement*> building;



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

	virtual void mouseDown(cocos2d::Event* event);


	// a selector callback
	void menuReturn(cocos2d::Ref* pSender);

	//a auto-inherited schedual update function
	virtual void update(float dt);

	// implement the "static create()" method manually
	CREATE_FUNC(Game);

};

#endif // __GAME_H__
