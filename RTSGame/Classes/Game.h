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
	
	//层
	Layer* mouseLayer;
	Layer* game;
	
	//地图
	//@map层级：
	//@  10   建筑人物等
	//@	 100  鼠标
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
	Point visibleSize;
	Point mapSize;
	Point tmSize;

	//@屏幕坐标转换成层坐标（世界坐标系）
	Point convertToMapLayer(Point position)
	{
		position = position - map->getPosition();
		return position;
	}
	//@世界坐标转换成瓦片地图坐标
	Point convertToTiledMap(Point position)
	{
		position.y = mapSize.y - position.y;

		Point tmPoint;
		int addx = 0, addy = 0;
		if ((int)position.x % (int)tmSize.x > tmSize.x / 2) addx++;
		if ((int)position.y % (int)tmSize.y > tmSize.y / 2) addy++;
		tmPoint.x = (int)(position.x / tmSize.x) + addx;
		tmPoint.y = (int)(position.y / tmSize.y) - addy;
		return tmPoint;
	}
	//@瓦片转换成世界坐标
	Point convertFromTMToWorld(Point position)
	{
		position.x *= tmSize.x;
		position.y *= tmSize.y;
		position.y = mapSize.y - position.y;
		return position;
	}


	//事件
	//@基本信息
	int myTeam;
	int resourceMoney;
	int resourcePower;
	//@游戏状态 - 0-off 1-on 2-building
	int selectedState;
	int buildState;
	
	//某些操作功能
	//@范围判断 - 注意保证坐标系一致
	bool rectContain(Rect rect,Point position)
	{
		if (position.x >= rect.getMinX() && position.x <= rect.getMaxX() && position.y >= rect.getMinY() &&position.y <= rect.getMaxY())
			return true;
		else
			return false;
	}

	//@onMouseMove:
	//@移动界面
	void onMouseMove(cocos2d::Event* event);
	void onMouseDown(cocos2d::Event* event);
	
	//@建筑状态
	Sprite* BuildingPictureWithMouse;

	//@菜单事件
	Menu* menu;
	Rect menuRect;

	MenuItemImage* basement_button;
	MenuItemImage* basementx_button;
	MenuItemImage* barrack_button;
	MenuItemImage* barrackx_button;
	MenuItemImage* powerplant_button;
	MenuItemImage* powerplantx_button;
	MenuItemImage* minefield_button;
	MenuItemImage* minefieldx_button;
	MenuItemImage* warfactory_button;
	MenuItemImage* warfactoryx_button;
	MenuItemImage* infantry_button;
	MenuItemImage* infantryx_button;
	MenuItemImage* dog_button;
	MenuItemImage* dogx_button;
	MenuItemImage* tank_button;
	MenuItemImage* tankx_button;

	//@创建建筑/人物
	//@BUILDING - 描述建筑状态和建筑信息
	int BUILDING;

	void buttonBasement(Ref* pSender);
	void buttonBarrack(Ref* pSender);
	void buttonPowerplant(Ref* pSender);
	void buttonMinefield(Ref* pSender);
	void buttonWarfactory(Ref* pSender);


	void buttonx(Ref* pSender);



	//Update周期函数
	virtual void update(float dt);
	void menuUpdate();

	//场景切换
	void menuReturn(cocos2d::Ref* pSender);

	// implement the "static create()" method manually
	CREATE_FUNC(Game);

};

#endif // __GAME_H__
