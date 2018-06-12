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
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	//地图
	Vec2 visibleSize;

	TMXTiledMap* map;
	TMXLayer* _grass;
	TMXLayer* _grasswet;
	TMXLayer* _ground;
	TMXLayer* _sea;
	TMXLayer* _meta;

	//元地图
	//@empty 0 @full 1
	bool readBlock(Point position);
	//
	void setBlock(Point position);

	//坐标转换
	Point convertToTiledMap(Point position);


	//事件
	virtual void mouseDown(cocos2d::Event* event);



	//a auto-inherited schedual update function
	virtual void update(float dt);

	//场景切换
	void menuReturn(cocos2d::Ref* pSender);

	// implement the "static create()" method manually
	CREATE_FUNC(Game);

};

#endif // __GAME_H__
