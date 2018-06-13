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
	TMXTiledMap* map;
	TMXLayer* _grass;
	TMXLayer* _grasswet;
	TMXLayer* _ground;
	TMXLayer* _sea;
	TMXLayer* _meta;

	//元地图属性
	//@empty 0 @full 1
	bool readBlock(Point position);
	void addBlock(Point position);
	void removeBlock(Point position);

	//坐标转换
	//@visibleSize  窗口可见大小
	//@mapSize      地图大小
	//@tmSize       地图瓦片大小
	//@viewPoint    当前视野的左下角在世界坐标系上的位置
	Point visibleSize;
	Point mapSize;
	Point tmSize;
	Point viewPoint;

	//@屏幕坐标转换成层坐标（世界坐标系）
	Point convertToMapLayer(Point position)
	{
		position.x += viewPoint.x;
		position.y += viewPoint.y;
		return position;
	}
	//@世界坐标转换成瓦片地图坐标
	Point convertToTiledMap(Point position)
	{
		Point tmPoint;
		tmPoint.x = position.x / tmSize.x;
		tmPoint.y = (mapSize.y - position.y) / tmSize.y;
		return tmPoint;
	}

	//事件
	//@游戏状态 - 0-off 1-on 2-building 
	int buildState;

	//@onMouseMove:
	//@移动界面
	void onMouseMove(cocos2d::Event* event);

	//@菜单事件
	//@创建建筑/人物
	//@BUILDING - 描述建筑状态和建筑信息
	int BUILDING;
	void buttonBasement(Ref* pSender);
	void buttonBasementx(Ref* pSender);



	//Update周期函数
	virtual void update(float dt);

	//场景切换
	void menuReturn(cocos2d::Ref* pSender);

	// implement the "static create()" method manually
	CREATE_FUNC(Game);

};

#endif // __GAME_H__
