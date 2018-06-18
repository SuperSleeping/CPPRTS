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
	TMXLayer* _meta;

	//元地图属性
	//@empty 0 @full 1
	bool isTileOutOfRange(Point tmPoint)
	{
		if (0 <= tmPoint.x&&tmPoint.x < tmNumber.x && 0 <= tmPoint.y&&tmPoint.y < tmNumber.y)
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	bool readBlock(Point tmPoint);
	void addBlock(Point tmPoint);
	void removeBlock(Point tmPoint);
	//@元地图Block属性用isBlock数组保存 便于修改
	bool isBlock[118][138];
	void isBlockInitialize();

	//坐标转换
	//@visibleSize  窗口可见大小
	//@mapSize      地图大小
	//@tmSize       地图瓦片大小
	Point visibleSize;
	Point mapSize;
	Point tmSize;
	Point tmNumber;

	//@屏幕坐标转换成层坐标（世界坐标系）
	Point convertToMapLayer(Point position)
	{
		position = position - map->getPosition();
		return position;
	}
	//@世界坐标转换成瓦片地图坐标
	Point convertToTiledMap(Point position)
	{
		int x, y;
		x = position.x / tmSize.x;
		y = (mapSize.y - position.y) / tmSize.y;
		return Vec2(x, y);
	}
	//@瓦片转换成世界坐标
	Point convertFromTMToWorld(Point position)
	{
		position.x *= tmSize.x;
		position.y *= tmSize.y;
		position.y = mapSize.y - position.y;
		return position;
	}
	//@世界坐标换成相邻的瓦片坐标
	Point convertToNeightborTiledMap(Point position)
	{
		Point tmPoint;
		tmPoint = convertToTiledMap(position);
		int addx = 0, addy = 0;
		float _y = mapSize.y - position.y;

		if (((int)position.x % (int)tmSize.x) >= tmSize.x / 2)addx++;
		if (((int)_y % (int)tmSize.y) >= tmSize.y / 2)addy++;

		tmPoint += Vec2(addx, addy);

		return tmPoint;
	}


	//事件
	//@基本信息
	int myTeam;
	int resourceMoney;
	int resourcePower;
	//@游戏状态 - 0-off 1-on 2-building
	bool selectedState;			//0非选择状态/1选择状态
	int selectedType;			//0无选择对象/选择的建筑类型（改变菜单）/选择人物状态
	Point selectedSpawnPoint;
	int buildState;				//0非建筑状态/预建设建筑类型
	bool OKtobuilt;

	//某些操作功能
	//@范围判断 - 注意保证坐标系一致
	bool rectContain(Rect rect,Point position)
	{
		if (position.x >= rect.getMinX() && position.x <= rect.getMaxX() && position.y >= rect.getMinY() &&position.y <= rect.getMaxY())
			return true;
		else
			return false;
	}

	//@碰撞检测
	//@建造检测

	//      *
	//    * * *
	//      *		occupiedCoordinate[0]
	//      *
	//    * * *
	//  * * * * *
	//    * * *
	//      *		occupiedCoordinate[1]
	vector<Vec2> occupiedCoordinate[2];
	vector<Vec2> checkOccupiedCoordinate[2];
	vector<Vec2>::iterator iter;
	void occupiedRelatedCoordinateInitialize()
	{
		occupiedCoordinate[0].push_back(Vec2(-1, 0));
		occupiedCoordinate[0].push_back(Vec2(0, -1));
		occupiedCoordinate[0].push_back(Vec2(0, 0));
		occupiedCoordinate[0].push_back(Vec2(0, 1));
		occupiedCoordinate[0].push_back(Vec2(1, 0));

		occupiedCoordinate[1].push_back(Vec2(-2, 0));
		occupiedCoordinate[1].push_back(Vec2(-1, -1));
		occupiedCoordinate[1].push_back(Vec2(-1, 0));
		occupiedCoordinate[1].push_back(Vec2(-1, 1));
		occupiedCoordinate[1].push_back(Vec2(0, -2));
		occupiedCoordinate[1].push_back(Vec2(0, -1));
		occupiedCoordinate[1].push_back(Vec2(0, 0));
		occupiedCoordinate[1].push_back(Vec2(0, 1));
		occupiedCoordinate[1].push_back(Vec2(0, 2));
		occupiedCoordinate[1].push_back(Vec2(1, -1));
		occupiedCoordinate[1].push_back(Vec2(1, 0));
		occupiedCoordinate[1].push_back(Vec2(1, 1));
		occupiedCoordinate[1].push_back(Vec2(2, 0));

		checkOccupiedCoordinate[0].push_back(Vec2(0, 0));

		checkOccupiedCoordinate[1].push_back(Vec2(-1, 0));
		checkOccupiedCoordinate[1].push_back(Vec2(0, -1));
		checkOccupiedCoordinate[1].push_back(Vec2(0, 0));
		checkOccupiedCoordinate[1].push_back(Vec2(0, 1));
		checkOccupiedCoordinate[1].push_back(Vec2(1, 0));

	}

	//@碰撞检测：建筑范围内瓦片的属性
	bool buildingTypeJudge(int x)
	{
		if (x == 2)
		{
			return 0;
		}
		else return 1;
	}
	bool readOccupiedTile(Point tmPoint, int buildingType);		//读所占空间是否已被占有，判断是否能新建	//占有-1 未占有-0
	void changeOccupiedTile(Point tmPoint, int buildingType);	//新建后改变地图属性

	//鼠标事件
	//@三种状态
	//@已包含功能：
	//
	//******Up:
	//		*(单点选择某Building/Character)
	//
	//******Down：
	//		*(框选Character)
	//		建一个新建筑
	//
	//******Move:
	//		视角切换
	//		建筑状态下，选定的可建筑类型跟随鼠标移动
	void onMouseMove(cocos2d::Event* event);
	void onMouseDown(cocos2d::Event* event);
	void onMouseUp(cocos2d::Event* event);

	//@框选范围
	Point firstPress;
	Point lastPress;
	Rect selectRect;
	DrawNode* rectangle;
	void drawline();
	
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
	void buttonInfantry(Ref* pSender);
	void buttonDog(Ref* pSender);
	void buttonTank(Ref* pSender);

	void buttonx(Ref* pSender);

	//





	//Update周期函数
	virtual void update(float dt);
	void menuUpdate();

	//场景切换
	void menuReturn(cocos2d::Ref* pSender);

	// implement the "static create()" method manually
	CREATE_FUNC(Game);

};


#endif // __GAME_H__
