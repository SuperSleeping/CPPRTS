#include "Game.h"
#include "SimpleAudioEngine.h"
#include "HelloWorldScene.h"
#include <iostream>
#include "ui/CocosGUI.h"
#include "UserDefault.h"
#include "extensions/cocos-ext.h"

#include"GameElement/Basement.h"
#include"GameElement/Barrack.h"
#include"GameElement/Warfactory.h"
#include"GameElement/Minefield.h"
#include"GameElement/Powerplant.h"

#include"GameElement/Infantry.h"
#include"GameElement/Dog.h"
#include"GameElement/Tank.h"

#include"GameInformation.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace CocosDenshion;
using namespace cocos2d::ui;


//建立vector储存gameElement
//4个数组分别存放四个国家的兵力和建筑


vector<Infantry*> infantryGroup[4];
vector<Dog*> dogGroup[4];
vector<Tank*> tankGroup[4];

vector<Basement*> basementGroup[4];
vector<Barrack*> barrackGroup[4];
vector<Warfactory*> warfactoryGroup[4];
vector<Minefield*> minefieldGroup[4];
vector<Powerplant*> powerplantGroup[4];

int MapInfo[118][138];
int Block[118][138];
int Characters[118][138];
int Buildings[118][138];
int MapCondition[118][138];

vector<int[118][138]> MapDestination;

Vec2 Safe(Vec2 &position);

void MapBlockBegin();
void BuildBlock(int x, int y, int size);
void RefreshMap(int map[118][138]);

//当前玩家的资源数据
int money;
int electricity;

//地图信息

//地图
//@map层级：
//@  10   建筑人物等
//@	 100  鼠标
TMXTiledMap* map;
TMXLayer* _grass;
TMXLayer* _grasswet;
TMXLayer* _ground;
TMXLayer* _meta;

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
//判断目标是否超出瓦片地图的范围
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

Scene* Game::createScene()
{
	auto scene = Game::create();

	return scene;
}

bool Game::init()
{
	if (!Scene::init())
	{
		return false;
	}

	std::string hostIp = UserDefault::getInstance()->getStringForKey(HOST_IP);
	sioClient = cocos2d::network::SocketIO::connect(hostIp, *this);
	sioClient->on("numberClientEvent", CC_CALLBACK_2(Game::numberClientEvent, this));

	visibleSize = Director::getInstance()->getVisibleSize();

	//添加层(?)
	//////#############################################################################!!!!!!!!解决一下回调函数监听器什么的捆绑在哪个node上的问题
	auto menuLayer = Layer::create();
	mouseLayer = Layer::create();
	game = Layer::create();
	this->addChild(menuLayer, 1);
	this->addChild(mouseLayer, 2);

	//置入地图
	map = TMXTiledMap::create("map/maptest.tmx");
	map->addChild(game, 100);
	this->addChild(map, 0);

	_grass = map->getLayer("grass");
	_grasswet = map->getLayer("grasswet");
	_ground = map->getLayer("ground");
	_meta = map->getLayer("meta");

	rectangle = DrawNode::create();
	map->addChild(rectangle, 100);

	//基本信息、尺寸、坐标信息初始化
	myTeam = 0;
	visibleSize.x = 1600;
	visibleSize.y = 900;
	tmSize.x = map->getTileSize().width;
	tmSize.y = map->getTileSize().height;
	mapSize.x = map->getMapSize().width * tmSize.x;
	mapSize.y = map->getMapSize().height * tmSize.y;
	tmNumber.x = mapSize.x / tmSize.x;
	tmNumber.y = mapSize.y / tmSize.y;
	OKtobuilt = 0;
	occupiedRelatedCoordinateInitialize();
	isBlockInitialize();

	MapBlockBegin();
	
	//测试用攻击对象
	auto character = Infantry::create(selectedSpawnPoint);
	int z = convertToNeightborTiledMap(selectedSpawnPoint).y;
	game->addChild(character, z);
	character->setTeam(1);
	infantryGroup[character->team].push_back(character);
	character->positionGoal = Vec2(20, 100);
	character->setPosition(Vec2(200, 200));
	character->schedule(schedule_selector(Character::updateMove), 0.01f, kRepeatForever, 0);
	character->setMapDestination(Vec2(20, 100));
	character->setTag(CreateTag);
	CreateTag++;

	auto building = Basement::create(convertFromTMToWorld(Vec2(20, 120)));
	building->setTeam(1);
	basementGroup[building->team].push_back(building);
	game->addChild(building, 120);
	BuildBlock(20, 120, 2);
	building->setTag(CreateTag);
	CreateTag++;

	//菜单栏
	{
		menu = Menu::create();
		//按钮的z坐标决定图像和触发函数 z坐标较大的图像显示在上层 但是先调用的是z坐标较小的函数
		//解决办法是改变按钮是否可按的状态
		basement_button = MenuItemImage::create("Game/button/basement_button1.png", "Game/button/basement_button3.png", CC_CALLBACK_1(Game::buttonBasement, this));
		basementx_button = MenuItemImage::create("Game/button/basement_button2.png", "Game/button/basement_button2.png", CC_CALLBACK_1(Game::buttonx, this));
		barrack_button = MenuItemImage::create("Game/button/barrack_button1.png", "Game/button/barrack_button3.png", CC_CALLBACK_1(Game::buttonBarrack, this));
		barrackx_button = MenuItemImage::create("Game/button/barrack_button2.png", "Game/button/barrack_button2.png", CC_CALLBACK_1(Game::buttonx, this));
		powerplant_button = MenuItemImage::create("Game/button/powerplant_button1.png", "Game/button/powerplant_button3.png", CC_CALLBACK_1(Game::buttonPowerplant, this));
		powerplantx_button = MenuItemImage::create("Game/button/powerplant_button2.png", "Game/button/powerplant_button2.png", CC_CALLBACK_1(Game::buttonx, this));
		minefield_button = MenuItemImage::create("Game/button/minefield_button1.png", "Game/button/minefield_button3.png", CC_CALLBACK_1(Game::buttonMinefield, this));
		minefieldx_button = MenuItemImage::create("Game/button/minefield_button2.png", "Game/button/minefield_button2.png", CC_CALLBACK_1(Game::buttonx, this));
		warfactory_button = MenuItemImage::create("Game/button/warfactory_button1.png", "Game/button/warfactory_button3.png", CC_CALLBACK_1(Game::buttonWarfactory, this));
		warfactoryx_button = MenuItemImage::create("Game/button/warfactory_button2.png", "Game/button/warfactory_button2.png", CC_CALLBACK_1(Game::buttonx, this));
		infantry_button = MenuItemImage::create("Game/button/infantry_button1.png", "Game/button/infantry_button3.png", CC_CALLBACK_1(Game::buttonInfantry, this));
		infantryx_button = MenuItemImage::create("Game/button/infantry_button2.png", "Game/button/infantry_button2.png", CC_CALLBACK_1(Game::buttonx, this));
		dog_button = MenuItemImage::create("Game/button/dog_button1.png", "Game/button/dog_button3.png", CC_CALLBACK_1(Game::buttonDog, this));
		dogx_button = MenuItemImage::create("Game/button/dog_button2.png", "Game/button/dog_button2.png", CC_CALLBACK_1(Game::buttonx, this));
		tank_button = MenuItemImage::create("Game/button/tank_button1.png", "Game/button/tank_button3.png", CC_CALLBACK_1(Game::buttonTank, this));
		tankx_button = MenuItemImage::create("Game/button/tank_button2.png", "Game/button/tank_button2.png", CC_CALLBACK_1(Game::buttonx, this));

		//锚点坐标设置
		basement_button->setAnchorPoint(Vec2(0, 0));
		basementx_button->setAnchorPoint(Vec2(0, 0));
		barrack_button->setAnchorPoint(Vec2(0, 0));
		barrackx_button->setAnchorPoint(Vec2(0, 0));
		powerplant_button->setAnchorPoint(Vec2(0, 0));
		powerplantx_button->setAnchorPoint(Vec2(0, 0));
		minefield_button->setAnchorPoint(Vec2(0, 0));
		minefieldx_button->setAnchorPoint(Vec2(0, 0));
		warfactory_button->setAnchorPoint(Vec2(0, 0));
		warfactoryx_button->setAnchorPoint(Vec2(0, 0));
		infantry_button->setAnchorPoint(Vec2(0, 0));
		infantryx_button->setAnchorPoint(Vec2(0, 0));
		dog_button->setAnchorPoint(Vec2(0, 0));
		dogx_button->setAnchorPoint(Vec2(0, 0));
		tank_button->setAnchorPoint(Vec2(0, 0));
		tankx_button->setAnchorPoint(Vec2(0, 0));
		//坐标设置
		basement_button->setPosition(Vec2(0, 0));
		basementx_button->setPosition(Vec2(0, 0));
		barrack_button->setPosition(Vec2(100, 0));
		barrackx_button->setPosition(Vec2(100, 0));
		powerplant_button->setPosition(Vec2(200, 0));
		powerplantx_button->setPosition(Vec2(200, 0));
		minefield_button->setPosition(Vec2(300, 0));
		minefieldx_button->setPosition(Vec2(300, 0));
		warfactory_button->setPosition(Vec2(400, 0));
		warfactoryx_button->setPosition(Vec2(400, 0));
		infantry_button->setPosition(Vec2(200, 100));
		infantryx_button->setPosition(Vec2(200, 100));
		dog_button->setPosition(Vec2(300, 100));
		dogx_button->setPosition(Vec2(300, 100));
		tank_button->setPosition(Vec2(400, 100));
		tankx_button->setPosition(Vec2(400, 100));
		//按钮加入菜单
		menu->addChild(basement_button, 0);
		menu->addChild(basementx_button, 1);
		menu->addChild(barrack_button, 0);
		menu->addChild(barrackx_button, 1);
		menu->addChild(powerplant_button, 0);
		menu->addChild(powerplantx_button, 1);
		menu->addChild(minefield_button, 0);
		menu->addChild(minefieldx_button, 1);
		menu->addChild(warfactory_button, 0);
		menu->addChild(warfactoryx_button, 1);
		menu->addChild(infantry_button, 0);
		menu->addChild(infantryx_button, 1);
		menu->addChild(dog_button, 0);
		menu->addChild(dogx_button, 1);
		menu->addChild(tank_button, 0);
		menu->addChild(tankx_button, 1);

		//菜单范围
		menuRect = Rect(visibleSize.x - 500, 0, 500, 200);

		menu->setPosition(Vec2(visibleSize.x - 500, 0));

		menuLayer->addChild(menu);
	}

	//触发事件
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	//@鼠标移动到边界的时候屏幕移动
	auto mouseListener = EventListenerMouse::create();


	mouseListener->onMouseMove = CC_CALLBACK_1(Game::onMouseMove, this);
	mouseListener->onMouseDown = CC_CALLBACK_1(Game::onMouseDown, this);
	mouseListener->onMouseUp = CC_CALLBACK_1(Game::onMouseUp, this);

	dispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);

	






	/*
	//GameScene上的鼠标监听、事件分配
	auto mouseListener = EventListenerMouse::create();
	mouseListener->onMouseDown = CC_CALLBACK_1(Game::mouseDown, this);
	dispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);
	*/

	this->scheduleUpdate();
	this->schedule(schedule_selector(Game::updateMapCharacter), 0.01f, kRepeatForever, 0);
	this->schedule(schedule_selector(Game::updateZOrder), 0.1f, kRepeatForever, 0);

	return true;
}

//更新函数
void Game::update(float dt)
{
	menuUpdate();
}

void Game::menuUpdate()
{
	//检测是否为某建筑的选中状态，再根据相应功能检测资源改变菜单
	if (selectedType == NULL)
	{

	}
	else if (selectedType == Building::BuildingType::BARRACK)
	{

	}
	else if (selectedType == Building::BuildingType::WARFACTORY)
	{

	}
	else
	{

	}
}

/***************/
//Player events
//Constructions:
/***************/
void Game::onMouseMove(cocos2d::Event* event)
{
	EventMouse* e = (EventMouse*)event;

	//右键监控
	//@排除右键操控
	if (e->getMouseButton() == EventMouse::MouseButton::BUTTON_RIGHT)return;

	Vec2 position = e->getLocationInView();
	Vec2 positionWorld = convertToMapLayer(position);

	//视野移动
	if (!rectContain(menuRect, position))
	{
		if (position.x > visibleSize.x - 100)
		{
			if (!(map->numberOfRunningActions()))
			{
				auto map_move = MoveTo::create((map->getPosition().x + (mapSize.x - visibleSize.x)) / 2000, Vec2(-(mapSize.x - visibleSize.x), map->getPosition().y));
				map->runAction(map_move);
			}
		}
		else if (position.x < 100)
		{
			if (!(map->numberOfRunningActions()))
			{
				auto map_move = MoveTo::create((-(map->getPosition().x)) / 2000, Vec2(0, map->getPosition().y));
				map->runAction(map_move);
			}
		}
		else if (position.y < 100)
		{
			if (!(map->numberOfRunningActions()))
			{
				auto map_move = MoveTo::create((-(map->getPosition().y)) / 2000, Vec2(map->getPosition().x, 0));
				map->runAction(map_move);
			}
		}
		else if (position.y > visibleSize.y - 100)
		{
			if (!(map->numberOfRunningActions()))
			{
				auto map_move = MoveTo::create((map->getPosition().y + (mapSize.y - visibleSize.y)) / 2000, Vec2(map->getPosition().x, -(mapSize.y - visibleSize.y)));
				map->runAction(map_move);
			}
		}
		else
		{
			map->stopAllActions();
		}
	}

	//建筑状态
	if (buildState)
	{
		//随鼠标移动的透明图片
		//移除上一个精灵
		if (BuildingPictureWithMouse != NULL)
		{
			map->removeChild(BuildingPictureWithMouse);
		}

		//检测碰撞(OKtobuilt)
		positionWorld = convertToNeightborTiledMap(positionWorld);
		OKtobuilt = 1 - readOccupiedTile(positionWorld, buildState);

		//创建随鼠标移动的图片精灵
		if (buildState == Building::BuildingType::BASEMENT)
		{
			if (OKtobuilt)
			{
				BuildingPictureWithMouse = Sprite::create("Game/building/basement.png");
			}
			else
			{
				BuildingPictureWithMouse = Sprite::create("Game/building/basement_block.png");
			}
			BuildingPictureWithMouse->setAnchorPoint(Vec2(0.5, 0.3));
		}
		else if (buildState == Building::BuildingType::BARRACK)
		{
			if (OKtobuilt)
			{
				BuildingPictureWithMouse = Sprite::create("Game/building/barrack.png");
			}
			else
			{
				BuildingPictureWithMouse = Sprite::create("Game/building/barrack_block.png");
			}
			BuildingPictureWithMouse->setAnchorPoint(Vec2(0.5, 0.4));
		}
		else if (buildState == Building::BuildingType::MINEFIELD)
		{
			if (OKtobuilt)
			{
				BuildingPictureWithMouse = Sprite::create("Game/building/minefield.png");
			}
			else
			{
				BuildingPictureWithMouse = Sprite::create("Game/building/minefield_block.png");
			}
			BuildingPictureWithMouse->setAnchorPoint(Vec2(0.5, 0.25));
		}
		else if (buildState == Building::BuildingType::POWERPLANT)
		{
			if (OKtobuilt)
			{
				BuildingPictureWithMouse = Sprite::create("Game/building/powerplant.png");
			}
			else
			{
				BuildingPictureWithMouse = Sprite::create("Game/building/powerplant_block.png");
			}
			BuildingPictureWithMouse->setAnchorPoint(Vec2(0.5, 0.2));
		}
		else if (buildState == Building::BuildingType::WARFACTORY)
		{
			if (OKtobuilt)
			{
				BuildingPictureWithMouse = Sprite::create("Game/building/warfactory.png");
			}
			else
			{
				BuildingPictureWithMouse = Sprite::create("Game/building/warfactory_block.png");
			}
			BuildingPictureWithMouse->setAnchorPoint(Vec2(0.5, 0.4));
		}

		//在瓦片地图上定位 探测可能安放的位置
		Point possiblePosition = convertFromTMToWorld(positionWorld);
		BuildingPictureWithMouse->setPosition(possiblePosition);
		BuildingPictureWithMouse->setOpacity(150);
		map->addChild(BuildingPictureWithMouse, 100);
	}

	//选择状态
	else if (selectedState)
	{
		//lastPress载入
		lastPress = positionWorld;
		drawline();
	}
}

void Game::onMouseDown(cocos2d::Event* event)
{
	EventMouse* e = (EventMouse*)event;

	//右键监控
	//@直接恢复初始状态
	if (e->getMouseButton() == EventMouse::MouseButton::BUTTON_RIGHT)
	{
		buildState = false;
		map->removeChild(BuildingPictureWithMouse);
		selectedState = false;
		selectedType = NULL;
		rectangle->clear();

		//清除所有选择标志
		//遍历建筑
		{
			//Basement
			{
				vector<Basement*>::iterator iterBasement;
				for (iterBasement = basementGroup[myTeam].begin(); iterBasement != basementGroup[myTeam].end(); iterBasement++)
				{
					(*iterBasement)->setSelected(false);
				}
			}
			//Minefield
			{
				vector<Minefield*>::iterator iterMinefield;
				for (iterMinefield = minefieldGroup[myTeam].begin(); iterMinefield != minefieldGroup[myTeam].end(); iterMinefield++)
				{
					(*iterMinefield)->setSelected(false);
				}
			}
			//Barrack
			{
				vector<Barrack*>::iterator iterBarrack;
				for (iterBarrack = barrackGroup[myTeam].begin(); iterBarrack != barrackGroup[myTeam].end(); iterBarrack++)
				{
					(*iterBarrack)->setSelected(false);
				}
			}
			//Warfactory
			{
				vector<Warfactory*>::iterator iterWarfactory;
				for (iterWarfactory = warfactoryGroup[myTeam].begin(); iterWarfactory != warfactoryGroup[myTeam].end(); iterWarfactory++)
				{
					(*iterWarfactory)->setSelected(false);
				}
			}
			//Powerplant
			{
				vector<Powerplant*>::iterator iterPowerplant;
				for (iterPowerplant = powerplantGroup[myTeam].begin(); iterPowerplant != powerplantGroup[myTeam].end(); iterPowerplant++)
				{
					(*iterPowerplant)->setSelected(false);
				}
			}
		}
		//遍历人物
		{
			//Infantry
			vector<Infantry*>::iterator iterInfantry;
			for (iterInfantry = infantryGroup[myTeam].begin(); iterInfantry != infantryGroup[myTeam].end(); iterInfantry++)
			{
				(*iterInfantry)->setSelected(false);
			}
			//dog
			vector<Dog*>::iterator iterDog;
			for (iterDog = dogGroup[myTeam].begin(); iterDog != dogGroup[myTeam].end(); iterDog++)
			{
				(*iterDog)->setSelected(false);
			}
			//tank
			vector<Tank*>::iterator iterTank;
			for (iterTank = tankGroup[myTeam].begin(); iterTank != tankGroup[myTeam].end(); iterTank++)
			{
				(*iterTank)->setSelected(false);
			}
		}
		return;
	}


	enum { screen, world, tiledmapTM, tiledmapW };
	Point position[4];
	position[screen] = e->getLocationInView();
	position[world] = convertToMapLayer(position[screen]);
	position[tiledmapTM] = convertToNeightborTiledMap(position[world]);
	position[tiledmapW] = convertFromTMToWorld(position[tiledmapTM]);

	//排除菜单范围
	if (rectContain(menuRect, position[screen]))return;

	//建筑状态
	if (buildState&&OKtobuilt)
	{
		//根据状态新建不同的GameElement并推入相应队伍的vector中
		if (buildState == Building::BuildingType::BASEMENT)
		{
			auto building = Basement::create(position[tiledmapW]);
			building->setTeam(myTeam);
			basementGroup[myTeam].push_back(building);
			game->addChild(building, position[tiledmapTM].y);
			BuildBlock(position[tiledmapTM].x, position[tiledmapTM].y, 2);
			building->setTag(CreateTag);
			CreateTag++;
		}
		else if (buildState == Building::BuildingType::BARRACK)
		{
			auto building = Barrack::create(position[tiledmapW]);
			building->setTeam(myTeam);
			barrackGroup[myTeam].push_back(building);
			game->addChild(building, position[tiledmapTM].y);
			BuildBlock(position[tiledmapTM].x, position[tiledmapTM].y, 2);
			building->setTag(CreateTag);
			CreateTag++;
		}
		else if (buildState == Building::BuildingType::MINEFIELD)
		{
			auto building = Minefield::create(position[tiledmapW]);
			building->setTeam(myTeam);
			minefieldGroup[myTeam].push_back(building);
			game->addChild(building, position[tiledmapTM].y);
			BuildBlock(position[tiledmapTM].x, position[tiledmapTM].y, 2);
			building->setTag(CreateTag);
			CreateTag++;
		}
		else if (buildState == Building::BuildingType::POWERPLANT)
		{
			auto building = Powerplant::create(position[tiledmapW]);
			building->setTeam(myTeam);
			powerplantGroup[myTeam].push_back(building);
			game->addChild(building, position[tiledmapTM].y);
			BuildBlock(position[tiledmapTM].x, position[tiledmapTM].y, 1);
			building->setTag(CreateTag);
			CreateTag++;
		}
		else if (buildState == Building::BuildingType::WARFACTORY)
		{
			auto building = Warfactory::create(position[tiledmapW]);
			building->setTeam(myTeam);
			warfactoryGroup[myTeam].push_back(building);
			game->addChild(building, position[tiledmapTM].y);
			BuildBlock(position[tiledmapTM].x, position[tiledmapTM].y, 2);
			building->setTag(CreateTag);
			CreateTag++;
		}
		else
		{
			log("Wrong : buildState not found!");
		}

		//添加建筑占地 改变地图属性
		changeOccupiedTile(position[tiledmapTM], buildState);

		//退出建筑状态
		buildState = NULL;
		map->removeChild(BuildingPictureWithMouse);
	}

	//选择状态（非建筑状态）
	else
	{
		//加载选择状态
		selectedState = true;
		//firstPress信息载入
		firstPress = position[world];
	}

}

void Game::onMouseUp(cocos2d::Event* event)
{
	EventMouse* e = (EventMouse*)event;

	//右键监控
	//@排除右键操控
	if (e->getMouseButton() == EventMouse::MouseButton::BUTTON_RIGHT)
		return;

	enum { screen, world, tiledmapTM, tiledmapW };
	Point position[4];
	position[screen] = e->getLocationInView();
	position[world] = convertToMapLayer(position[screen]);
	position[tiledmapTM] = convertToNeightborTiledMap(position[world]);
	position[tiledmapW] = convertFromTMToWorld(position[tiledmapTM]);
	int x = position[tiledmapTM].x;
	int y = position[tiledmapTM].y;





	bool swallow = 0;


	//遍历人物		//设成攻击目标
	{
		//由于人物比较小，所以改变selectedRect的大小
		Point leftdown = Vec2(position[world].x - 10, position[world].y - 30);
		Size size = Size(20, 40);
		selectRect = Rect(leftdown, size);
		int zo = 0;

		//Infantry
		for (int s = (myTeam + 1) % 2; s < 4; s += 2)
		{
			vector<Infantry*>::iterator iterInfantry;
			for (iterInfantry = infantryGroup[s].begin(); iterInfantry != infantryGroup[s].end(); iterInfantry++)
			{
				if (!(*iterInfantry)->died && rectContain(selectRect, (*iterInfantry)->getPosition()))
				{
					if ((*iterInfantry)->getZOrder() > zo)
					{
						zo = (*iterInfantry)->getZOrder();
						for (Infantry* character : infantryGroup[myTeam])
						{
							if (character->selected)
							{
								character->stopAllActions();
								character->attackTag = (*iterInfantry)->getTag();
								log("set tag: %d", (*iterInfantry)->getTag());
							}
						}
						for (Tank* character : tankGroup[myTeam])
						{
							if (character->selected)
							{
								character->stopAllActions();
								character->attackTag = (*iterInfantry)->getTag();
								log("set tag: %d", (*iterInfantry)->getTag());
							}
						}
						for (Dog* character : dogGroup[myTeam])
						{
							if (character->selected)
							{
								character->stopAllActions();
								character->attackTag = (*iterInfantry)->getTag();
								log("set tag: %d", (*iterInfantry)->getTag());
							}
						}
					}
					swallow = 1;
				}

			}
			//dog
			vector<Dog*>::iterator iterDog;
			for (iterDog = dogGroup[s].begin(); iterDog != dogGroup[s].end(); iterDog++)
			{
				if (!(*iterDog)->died && rectContain(selectRect, (*iterDog)->getPosition()))
				{
					if ((*iterDog)->getZOrder() > zo)
					{
						zo = (*iterDog)->getZOrder();
						for (Infantry* character : infantryGroup[myTeam])
						{
							if (character->selected)
							{
								character->stopAllActions();
								character->attackTag = (*iterDog)->getTag();
								log("set tag: %d", (*iterDog)->getTag());
							}
						}
						for (Tank* character : tankGroup[myTeam])
						{
							if (character->selected)
							{
								character->stopAllActions();
								character->attackTag = (*iterDog)->getTag();
								log("set tag: %d", (*iterDog)->getTag());
							}
						}
						for (Dog* character : dogGroup[myTeam])
						{
							if (character->selected)
							{
								character->stopAllActions();
								character->attackTag = (*iterDog)->getTag();
								log("set tag: %d", (*iterDog)->getTag());
							}
						}
					}
					swallow = 1;
				}
			}
			//tank
			vector<Tank*>::iterator iterTank;
			for (iterTank = tankGroup[s].begin(); iterTank != tankGroup[s].end(); iterTank++)
			{
				if (!(*iterTank)->died && rectContain(selectRect, (*iterTank)->getPosition()))
				{
					if ((*iterTank)->getZOrder() > zo)
					{
						zo = (*iterTank)->getZOrder();
						for (Infantry* character : infantryGroup[myTeam])
						{
							if (character->selected)
							{
								character->stopAllActions();
								character->attackTag = (*iterTank)->getTag();
								log("set tag: %d", (*iterTank)->getTag());
							}
						}
						for (Tank* character : tankGroup[myTeam])
						{
							if (character->selected)
							{
								character->stopAllActions();
								character->attackTag = (*iterTank)->getTag();
								log("set tag: %d", (*iterTank)->getTag());
							}
						}
						for (Dog* character : dogGroup[myTeam])
						{
							if (character->selected)
							{
								character->stopAllActions();
								character->attackTag = (*iterTank)->getTag();
								log("set tag: %d", (*iterTank)->getTag());
							}
						}
					}
					swallow = 1;
				}
			}
		}

		leftdown = Vec2(position[world].x - 48, position[world].y - 72);
		size = Size(96, 96);
		selectRect = Rect(leftdown, size);
		for (int s = (myTeam + 1) % 2; s < 4; s += 2)
		{


			//遍历建筑

			{
				//int zo = 0;
				//Basement
				{
					vector<Basement*>::iterator iterBasement;
					for (iterBasement = basementGroup[s].begin(); iterBasement != basementGroup[s].end(); iterBasement++)
					{
						if (!(*iterBasement)->died && rectContain(selectRect, (*iterBasement)->getPosition()))
						{
							if ((*iterBasement)->getZOrder() > zo)
							{
								zo = (*iterBasement)->getZOrder();
								for (Infantry* character : infantryGroup[myTeam])
								{
									if (character->selected)
									{
										character->stopAllActions();
										character->attackTag = (*iterBasement)->getTag();
										log("set tag: %d", (*iterBasement)->getTag());
									}
								}
								for (Tank* character : tankGroup[myTeam])
								{
									if (character->selected)
									{
										character->stopAllActions();
										character->attackTag = (*iterBasement)->getTag();
										log("set tag: %d", (*iterBasement)->getTag());
									}
								}
								for (Dog* character : dogGroup[myTeam])
								{
									if (character->selected)
									{
										character->stopAllActions();
										character->attackTag = (*iterBasement)->getTag();
										log("set tag: %d", (*iterBasement)->getTag());
									}
								}
							}
							swallow = 1;
						}
					}
				}
				//Minefield
				{
					vector<Minefield*>::iterator iterMinefield;
					for (iterMinefield = minefieldGroup[s].begin(); iterMinefield != minefieldGroup[s].end(); iterMinefield++)
					{
						if (!(*iterMinefield)->died && rectContain(selectRect, (*iterMinefield)->getPosition()))
						{
							if ((*iterMinefield)->getZOrder() > zo)
							{
								zo = (*iterMinefield)->getZOrder();
								for (Infantry* character : infantryGroup[myTeam])
								{
									if (character->selected)
									{
										character->stopAllActions();
										character->attackTag = (*iterMinefield)->getTag();
										log("set tag: %d", (*iterMinefield)->getTag());
									}
								}
								for (Tank* character : tankGroup[myTeam])
								{
									if (character->selected)
									{
										character->stopAllActions();
										character->attackTag = (*iterMinefield)->getTag();
										log("set tag: %d", (*iterMinefield)->getTag());
									}
								}
								for (Dog* character : dogGroup[myTeam])
								{
									if (character->selected)
									{
										character->stopAllActions();
										character->attackTag = (*iterMinefield)->getTag();
										log("set tag: %d", (*iterMinefield)->getTag());
									}
								}
							}
							swallow = 1;
						}
					}
				}
				//Barrack
				{
					vector<Barrack*>::iterator iterBarrack;
					for (iterBarrack = barrackGroup[s].begin(); iterBarrack != barrackGroup[s].end(); iterBarrack++)
					{
						if (!(*iterBarrack)->died && rectContain(selectRect, (*iterBarrack)->getPosition()))
						{
							if ((*iterBarrack)->getZOrder() > zo)
							{
								zo = (*iterBarrack)->getZOrder();
								for (Infantry* character : infantryGroup[myTeam])
								{
									if (character->selected)
									{
										character->stopAllActions();
										character->attackTag = (*iterBarrack)->getTag();
										log("set tag: %d", (*iterBarrack)->getTag());
									}
								}
								for (Tank* character : tankGroup[myTeam])
								{
									if (character->selected)
									{
										character->stopAllActions();
										character->attackTag = (*iterBarrack)->getTag();
										log("set tag: %d", (*iterBarrack)->getTag());
									}
								}
								for (Dog* character : dogGroup[myTeam])
								{
									if (character->selected)
									{
										character->stopAllActions();
										character->attackTag = (*iterBarrack)->getTag();
										log("set tag: %d", (*iterBarrack)->getTag());
									}
								}
							}
							swallow = 1;
						}
					}
				}
				//Warfactory
				{
					vector<Warfactory*>::iterator iterWarfactory;
					for (iterWarfactory = warfactoryGroup[s].begin(); iterWarfactory != warfactoryGroup[s].end(); iterWarfactory++)
					{
						if (!(*iterWarfactory) && rectContain(selectRect, (*iterWarfactory)->getPosition()))
						{
							if ((*iterWarfactory)->getZOrder() > zo)
							{
								zo = (*iterWarfactory)->getZOrder();
								for (Infantry* character : infantryGroup[myTeam])
								{
									if (character->selected)
									{
										character->stopAllActions();
										character->attackTag = (*iterWarfactory)->getTag();
										log("set tag: %d", (*iterWarfactory)->getTag());
									}
								}
								for (Tank* character : tankGroup[myTeam])
								{
									if (character->selected)
									{
										character->stopAllActions();
										character->attackTag = (*iterWarfactory)->getTag();
										log("set tag: %d", (*iterWarfactory)->getTag());
									}
								}
								for (Dog* character : dogGroup[myTeam])
								{
									if (character->selected)
									{
										character->stopAllActions();
										character->attackTag = (*iterWarfactory)->getTag();
										log("set tag: %d", (*iterWarfactory)->getTag());
									}
								}
							}
							swallow = 1;
						}
					}
				}
				//Powerplant
				{
					vector<Powerplant*>::iterator iterPowerplant;
					//Powerplant尺寸比较小 重新判断
					Point leftdown = Vec2(lastPress.x - 24, lastPress.y - 24);
					Size size = Size(48, 48);
					selectRect = Rect(leftdown, size);
					for (iterPowerplant = powerplantGroup[s].begin(); iterPowerplant != powerplantGroup[s].end(); iterPowerplant++)
					{
						if (!(*iterPowerplant)->died && rectContain(selectRect, (*iterPowerplant)->getPosition()))
						{
							if ((*iterPowerplant)->getZOrder() > zo)
							{
								zo = (*iterPowerplant)->getZOrder();
								for (Infantry* character : infantryGroup[myTeam])
								{
									if (character->selected)
									{
										character->stopAllActions();
										character->attackTag = (*iterPowerplant)->getTag();
										log("set tag: %d", (*iterPowerplant)->getTag());
									}
								}
								for (Tank* character : tankGroup[myTeam])
								{
									if (character->selected)
									{
										character->stopAllActions();
										character->attackTag = (*iterPowerplant)->getTag();
										log("set tag: %d", (*iterPowerplant)->getTag());
									}
								}
								for (Dog* character : dogGroup[myTeam])
								{
									if (character->selected)
									{
										character->stopAllActions();
										character->attackTag = (*iterPowerplant)->getTag();
										log("set tag: %d", (*iterPowerplant)->getTag());
									}
								}
							}
							swallow = 1;
						}
					}
				}
			}
		}

	}
	/*for (Infantry* enemy_character : infantryGroup[(myTeam + 1) % 2])
	{
		if (enemy_character->positionNow == position[tiledmapTM])
		{
			for (Infantry* character : infantryGroup[myTeam])
			{
				if (character->selected)
				{
					character->stopAllActions();
					character->attackTag = enemy_character->getTag();
					log("set tag: %d", enemy_character->getTag());
				}
			}
			swallow = 1;
			break;
		}
	}*/
	if (!swallow)
	{
		for (Infantry* character : infantryGroup[myTeam])
		{
			if (character->selected)
			{
				character->stopAllActions();
				//character->positionGoal = position[tiledmapTM];
				character->attackTag = 0;
				character->setGoal(Safe(position[tiledmapTM]));
				log("%d", character->getTag());
				//character->setMapDestination(position[tiledmapTM]);
				//character->schedule(schedule_selector(Character::updateMove), 0.01f, kRepeatForever, 0.0f);
				//character->schedule(schedule_selector(Character::updateAttack), 0.01f, kRepeatForever, 0.0f);

			}
		}
		for (Dog* character : dogGroup[myTeam])
		{
			if (character->selected)
			{
				character->stopAllActions();
				//character->positionGoal = position[tiledmapTM];
				character->attackTag = 0;
				character->setGoal(Safe(position[tiledmapTM]));

				//character->setMapDestination(position[tiledmapTM]);
				//character->schedule(schedule_selector(Character::updateMove), 0.01f, kRepeatForever, 0.0f);
				//character->schedule(schedule_selector(Character::updateAttack), 0.01f, kRepeatForever, 0.0f);

			}
		}
		for (Tank* character : tankGroup[myTeam])
		{
			if (character->selected)
			{
				character->stopAllActions();
				//character->positionGoal = position[tiledmapTM];
				character->attackTag = 0;
				character->setGoal(Safe(position[tiledmapTM]));

				//character->setMapDestination(position[tiledmapTM]);
				//character->schedule(schedule_selector(Character::updateMove), 0.01f, kRepeatForever, 0.0f);
				//character->schedule(schedule_selector(Character::updateAttack), 0.01f, kRepeatForever, 0.0f);

			}
		}
	}

	//建筑状态
	if (buildState) return;

	//选择状态
	if (selectedState)
	{


		//lastPress信息载入
		lastPress = position[world];
		//清除框选
		rectangle->clear();
		//开始置入当前次选择的情况
		selectedType = NULL;

		//判断是单选还是框选
		//单选
		//@判断标准：//目标范围包含鼠标点击点
		if (firstPress.x - lastPress.x <= 64 && firstPress.x - lastPress.x >= -64 && firstPress.y - lastPress.y <= 36 && firstPress.y - lastPress.y >= -36)
		{
			//以目标锚点为中心开始判断的矩形状
			//已经过数学变换
			Point leftdown = Vec2(lastPress.x - 48, lastPress.y - 72);
			Size size = Size(96, 96);
			selectRect = Rect(leftdown, size);

			//遍历建筑
			//@清除原来的select痕迹，创建新的select标记
			{
				//int zo = 0;
				//Basement
				{
					vector<Basement*>::iterator iterBasement;
					for (iterBasement = basementGroup[myTeam].begin(); iterBasement != basementGroup[myTeam].end(); iterBasement++)
					{
						if (rectContain(selectRect, (*iterBasement)->positionCurrent))
						{
							(*iterBasement)->setSelected(true);
							selectedState = true;
							selectedType = Building::BASEMENT;
						}
						else
						{
							(*iterBasement)->setSelected(false);
						}
					}
				}
				//Minefield
				{
					vector<Minefield*>::iterator iterMinefield;
					for (iterMinefield = minefieldGroup[myTeam].begin(); iterMinefield != minefieldGroup[myTeam].end(); iterMinefield++)
					{
						if (rectContain(selectRect, (*iterMinefield)->positionCurrent))
						{
							(*iterMinefield)->setSelected(true);
							selectedState = true;
							selectedType = Building::MINEFIELD;
						}
						else
						{
							(*iterMinefield)->setSelected(false);
						}
					}
				}
				//Barrack
				{
					vector<Barrack*>::iterator iterBarrack;
					for (iterBarrack = barrackGroup[myTeam].begin(); iterBarrack != barrackGroup[myTeam].end(); iterBarrack++)
					{
						if (rectContain(selectRect, (*iterBarrack)->positionCurrent))
						{
							(*iterBarrack)->setSelected(true);
							selectedState = true;
							selectedType = Building::BARRACK;
							selectedSpawnPoint = (*iterBarrack)->spawnPoint;
						}
						else
						{
							(*iterBarrack)->setSelected(false);
						}
					}
				}
				//Warfactory
				{
					vector<Warfactory*>::iterator iterWarfactory;
					for (iterWarfactory = warfactoryGroup[myTeam].begin(); iterWarfactory != warfactoryGroup[myTeam].end(); iterWarfactory++)
					{
						if (rectContain(selectRect, (*iterWarfactory)->positionCurrent))
						{
							(*iterWarfactory)->setSelected(true);
							selectedState = true;
							selectedType = Building::WARFACTORY;
							selectedSpawnPoint = (*iterWarfactory)->spawnPoint;
						}
						else
						{
							(*iterWarfactory)->setSelected(false);
						}
					}
				}
				//Powerplant
				{
					vector<Powerplant*>::iterator iterPowerplant;
					//Powerplant尺寸比较小 重新判断
					Point leftdown = Vec2(lastPress.x - 24, lastPress.y - 24);
					Size size = Size(48, 48);
					selectRect = Rect(leftdown, size);
					for (iterPowerplant = powerplantGroup[myTeam].begin(); iterPowerplant != powerplantGroup[myTeam].end(); iterPowerplant++)
					{
						if (rectContain(selectRect, (*iterPowerplant)->positionCurrent))
						{
							(*iterPowerplant)->setSelected(true);
							selectedState = true;
							selectedType = Building::POWERPLANT;
						}
						else
						{
							(*iterPowerplant)->setSelected(false);
						}
					}
				}
			}

			//遍历人物
			{
				//由于人物比较小，所以改变selectedRect的大小
				Point leftdown = Vec2(lastPress.x - 10, lastPress.y - 30);
				Size size = Size(20, 40);
				selectRect = Rect(leftdown, size);

				//Infantry
				vector<Infantry*>::iterator iterInfantry;
				for (iterInfantry = infantryGroup[myTeam].begin(); iterInfantry != infantryGroup[myTeam].end(); iterInfantry++)
				{
					if (rectContain(selectRect, (*iterInfantry)->getPosition()))
					{
						(*iterInfantry)->setSelected(true);
						selectedType = Character::CharacterChosen;
					}
					else
					{
						(*iterInfantry)->setSelected(false);
					}
				}
				//dog
				vector<Dog*>::iterator iterDog;
				for (iterDog = dogGroup[myTeam].begin(); iterDog != dogGroup[myTeam].end(); iterDog++)
				{
					if (rectContain(selectRect, (*iterDog)->getPosition()))
					{
						(*iterDog)->setSelected(true);
						selectedType = Character::CharacterChosen;
					}
					else
					{
						(*iterDog)->setSelected(false);
					}
				}
				//tank
				vector<Tank*>::iterator iterTank;
				for (iterTank = tankGroup[myTeam].begin(); iterTank != tankGroup[myTeam].end(); iterTank++)
				{
					if (rectContain(selectRect, (*iterTank)->getPosition()))
					{
						(*iterTank)->setSelected(true);
						selectedType = Character::CharacterChosen;
					}
					else
					{
						(*iterTank)->setSelected(false);
					}
				}
			}


		}

		//框选
		//@判断标准：框选范围内包括目标的瓦片地图位置
		else
		{
			//创建对应于框选的选择矩形
			{
				if (lastPress.x < firstPress.x&&lastPress.y < firstPress.y)
				{
					//左下角
					Size _size = Size(firstPress.x - lastPress.x, firstPress.y - lastPress.y);
					selectRect = Rect(lastPress, _size);
				}
				else if (lastPress.x<firstPress.x&&lastPress.y>firstPress.y)
				{
					//左上角
					Size _size = Size(firstPress.x - lastPress.x, lastPress.y - firstPress.y);
					Point _start = Vec2(lastPress.x, firstPress.y);
					selectRect = Rect(_start, _size);
				}
				else if (lastPress.x > firstPress.x&&lastPress.y > firstPress.y)
				{
					//右上角
					Size _size = Size(lastPress.x - firstPress.x, lastPress.y - firstPress.y);
					selectRect = Rect(firstPress, _size);
				}
				else if (lastPress.x > firstPress.x&&lastPress.y < firstPress.y)
				{
					//右下角
					Size _size = Size(lastPress.x - firstPress.x, firstPress.y - lastPress.y);
					Point _start = Vec2(firstPress.x, lastPress.y);
					selectRect = Rect(_start, _size);
				}
			}

			//遍历建筑
			{
				//Basement
				{
					vector<Basement*>::iterator iterBasement;
					for (iterBasement = basementGroup[myTeam].begin(); iterBasement != basementGroup[myTeam].end(); iterBasement++)
					{
						(*iterBasement)->setSelected(false);
					}
				}
				//Minefield
				{
					vector<Minefield*>::iterator iterMinefield;
					for (iterMinefield = minefieldGroup[myTeam].begin(); iterMinefield != minefieldGroup[myTeam].end(); iterMinefield++)
					{
						(*iterMinefield)->setSelected(false);
					}
				}
				//Barrack
				{
					vector<Barrack*>::iterator iterBarrack;
					for (iterBarrack = barrackGroup[myTeam].begin(); iterBarrack != barrackGroup[myTeam].end(); iterBarrack++)
					{
						(*iterBarrack)->setSelected(false);
					}
				}
				//Warfactory
				{
					vector<Warfactory*>::iterator iterWarfactory;
					for (iterWarfactory = warfactoryGroup[myTeam].begin(); iterWarfactory != warfactoryGroup[myTeam].end(); iterWarfactory++)
					{
						(*iterWarfactory)->setSelected(false);
					}
				}
				//Powerplant
				{
					vector<Powerplant*>::iterator iterPowerplant;
					for (iterPowerplant = powerplantGroup[myTeam].begin(); iterPowerplant != powerplantGroup[myTeam].end(); iterPowerplant++)
					{
						(*iterPowerplant)->setSelected(false);
					}
				}
			}

			//遍历人物
			{
				//Infantry
				vector<Infantry*>::iterator iterInfantry;
				for (iterInfantry = infantryGroup[myTeam].begin(); iterInfantry != infantryGroup[myTeam].end(); iterInfantry++)
				{
					if (rectContain(selectRect, (*iterInfantry)->getPosition()))
					{
						(*iterInfantry)->setSelected(true);
						selectedType = Character::CharacterChosen;
					}
					else
					{
						(*iterInfantry)->setSelected(false);
					}
				}
				//dog
				vector<Dog*>::iterator iterDog;
				for (iterDog = dogGroup[myTeam].begin(); iterDog != dogGroup[myTeam].end(); iterDog++)
				{
					if (rectContain(selectRect, (*iterDog)->getPosition()))
					{
						(*iterDog)->setSelected(true);
						selectedType = Character::CharacterChosen;
					}
					else
					{
						(*iterDog)->setSelected(false);
					}
				}
				//tank
				vector<Tank*>::iterator iterTank;
				for (iterTank = tankGroup[myTeam].begin(); iterTank != tankGroup[myTeam].end(); iterTank++)
				{
					if (rectContain(selectRect, (*iterTank)->getPosition()))
					{
						(*iterTank)->setSelected(true);
						selectedType = Character::CharacterChosen;
					}
					else
					{
						(*iterTank)->setSelected(false);
					}
				}
			}
		}

		//退出选择状态
		selectedState = false;

	}
	/*int nm = 1;
	for (Infantry* infa : infantryGroup[myTeam])
	{
		int x = infa->positionNow.x;
		int y = infa->positionNow.y;
		nm++;
		log("%d : %d,%d", nm, x, y);
		log("%d", Characters[x][y]);

	}*/




	int a = position[tiledmapTM].x;
	int b = position[tiledmapTM].y;
	log("BD :%d", Buildings[a][b]);

	//排除菜单范围


}

//框选的drawline
void Game::drawline()
{
	rectangle->clear();
	Vec2 point[4] =
	{
		firstPress,
		Vec2(firstPress.x,lastPress.y),
		lastPress,
		Vec2(lastPress.x,firstPress.y)
	};
	rectangle->drawPolygon(point, 4, Color4F(0, 0, 0, 0), 0.5, Color4F(1, 1, 1, 0.8));
}

/***************/
//菜单button
/***************/

void Game::buttonBasement(Ref* pSender)
{
	buildState = Building::BuildingType::BASEMENT;
}

void Game::buttonBarrack(Ref* pSender)
{
	buildState = Building::BuildingType::BARRACK;
}

void Game::buttonPowerplant(Ref* pSender)
{
	buildState = Building::BuildingType::POWERPLANT;
}

void Game::buttonMinefield(Ref* pSender)
{
	buildState = Building::BuildingType::MINEFIELD;
}

void Game::buttonWarfactory(Ref* pSender)
{
	buildState = Building::BuildingType::WARFACTORY;
}

void Game::buttonInfantry(Ref* pSender)
{
	auto character = Infantry::create(selectedSpawnPoint);
	int z = convertToNeightborTiledMap(selectedSpawnPoint).y;
	game->addChild(character, z);
	character->setTeam(myTeam);
	Gold[character->team] -= 500;
	infantryGroup[character->team].push_back(character);
	character->setTag(CreateTag);
	CreateTag++;
	log("%d Gold %d",myTeam, Gold[myTeam]);
}

void Game::buttonDog(Ref* pSender)
{
	auto character = Dog::create(selectedSpawnPoint);
	int z = convertToNeightborTiledMap(selectedSpawnPoint).y;
	game->addChild(character, z);
	character->setTeam(myTeam);
	Gold[character->team] -= 500;
	dogGroup[character->team].push_back(character);
	character->setTag(CreateTag);
	CreateTag++;
}

void Game::buttonTank(Ref* pSender)
{
	/*if (Gold[myTeam] < 1000)
	{
		return;
	}*/

	auto character = Tank::create(selectedSpawnPoint);
	int z = convertToNeightborTiledMap(selectedSpawnPoint).y;
	game->addChild(character, z);
	character->setTeam(myTeam);
	Gold[character->team] -= 1000;
	tankGroup[character->team].push_back(character);
	character->setTag(CreateTag);
	CreateTag++;
}

void Game::buttonx(Ref* pSender)
{
	log("Not enough resources or a wrong routine");
}

/***************/
//瓦片地图API
/***************/

void Game::isBlockInitialize()
{
	Point tmPoint;
	for (int x = 0; x < tmNumber.x; x++)
	{
		for (int y = 0; y < tmNumber.y; y++)
		{
			tmPoint = Vec2(x, y);

			//判断瓦片是否在范围之内
			if (!isTileOutOfRange(tmPoint))
			{
				int GID = _meta->getTileGIDAt(tmPoint);
				if (GID != 0)
				{
					auto properties = map->getPropertiesForGID(GID).asValueMap();
					if (!properties.empty())
					{
						auto block = properties["Block"].asString();
						if (block == "true")
						{
							isBlock[x][y] = true;
						}
						else
						{
							isBlock[x][y] = false;
						}
					}
				}
			}
		}
	}
}

bool Game::readBlock(Point tmPoint)
{
	int x = tmPoint.x;
	int y = tmPoint.y;

	//判断瓦片是否在范围之内
	if (0 <= x && x < tmNumber.x && 0 <= y && tmNumber.y)
	{
		return isBlock[x][y];
	}
	else
	{
		log("Error:Point out of range!");
		return 0;
	}
}

void Game::addBlock(Point tmPoint)
{
	int x = tmPoint.x;
	int y = tmPoint.y;

	//判断瓦片是否在范围之内
	if (0 <= x && x < tmNumber.x && 0 <= y && tmNumber.y)
	{
		isBlock[x][y] = true;
	}
	else
	{
		log("Error:Point out of range!");
	}
}

void Game::removeBlock(Point tmPoint)
{
	int x = tmPoint.x;
	int y = tmPoint.y;

	//判断瓦片是否在范围之内
	if (0 <= x && x < tmNumber.x && 0 <= y && tmNumber.y)
	{
		isBlock[x][y] = false;
	}
	else
	{
		log("Error:Point out of range!");
	}
}

//基于瓦片地图API的碰撞检测

bool Game::readOccupiedTile(Point tmPoint, int buildingType)
{
	int x = buildingTypeJudge(buildingType);
	Point middle = tmPoint;

	iter = checkOccupiedCoordinate[x].begin();
	for (iter; iter != checkOccupiedCoordinate[x].end(); iter++)
	{
		Point temp;
		temp.x = middle.x + (*iter).x;
		temp.y = middle.y + (*iter).y;
		//判断所在瓦片格子的Block属性
		//读到1说明格子Block属性为1 则不可建造
		if (readBlock(temp))
		{
			return true;
		}
	}
	return false;
}

void Game::changeOccupiedTile(Point tmPoint, int buildingType)
{
	int x = buildingTypeJudge(buildingType);
	Point middle = tmPoint;

	iter = occupiedCoordinate[x].begin();
	for (iter; iter != occupiedCoordinate[x].end(); iter++)
	{
		Point temp;
		temp.x = middle.x + (*iter).x;
		temp.y = middle.y + (*iter).y;
		//改变所在瓦片格子的Block属性
		addBlock(temp);

		//debug
		auto point = Sprite::create("point.png");
		point->setPosition(convertFromTMToWorld(temp));
		game->addChild(point, 500);
	}
}

void Game::menuReturn(cocos2d::Ref* pSender)
{

}
void MapBlockBegin()
{
	for (int i = 0; i < 118; i++) {
		for (int j = 0; j < 138; j++) {
			MapInfo[i][j] = 0;
			Block[i][j] = 0;
			Characters[i][j] = 0;
			Buildings[i][j] = 0;
		}
	}
	for (int i = 0; i < 118; i++)
	{
		Block[i][0] = -10000;

		Block[i][138] = -10000;
	}
	for (int i = 0; i < 138; i++)
	{
		Block[0][i] = -10000;;
		Block[118][i] = -10000;
	}
}

void BuildBlock(int x, int y, int size)
{
	log("zz");
	int q = 2;
	if (size == 2)
	{

		for (int i = x - 2; i <= x + 2; i++)
		{
			Buildings[i][y] = -1400;
		}
		for (int i = y - 2; i <= y + 2; i++)
		{
			Buildings[x][i] = -1400;
		}

		Buildings[x - 1][y - 1] = -1400;
		Buildings[x - 1][y + 1] = -1400;
		Buildings[x + 1][y - 1] = -1400;
		Buildings[x + 1][y + 1] = -1400;
		Buildings[x][y + 3] -= q;
		Buildings[x][y - 3] -= q;
		Buildings[x - 1][y + 2] -= q;
		Buildings[x - 1][y - 2] -= q;
		Buildings[x + 1][y + 2] -= q;
		Buildings[x + 1][y - 2] -= q;
		Buildings[x - 2][y + 1] -= q;
		Buildings[x - 2][y - 1] -= q;
		Buildings[x + 2][y + 1] -= q;
		Buildings[x + 2][y - 1] -= q;
		Buildings[x + 3][y] -= q;
		Buildings[x - 3][y] -= q;
	}
	else if (size == 1)
	{

		for (int i = x - 1; i <= x + 1; i++)
		{
			Buildings[i][y] = -1400;
		}
		for (int i = y - 1; i <= y + 1; i++)
		{
			Buildings[x][i] = -1400;
		}
		Buildings[x - 2][y] = -1400;
		Buildings[x + 2][y] = -1400;
		Buildings[x][y - 2] = -1400;
		Buildings[x - 1][y - 1] = -1400;
		Buildings[x - 1][y + 1] = -1400;
		Buildings[x + 1][y - 1] = -1400;
		Buildings[x + 1][y + 1] = -1400;

	}
}


void Game::updateMapCharacter(float di)
{
	for (int i = 0; i < 118; i++)
	{
		for (int j = 0; j < 138; j++)
		{
			Characters[i][j] = 0;
		}
	}
	for (int i = 0; i < 4; i++)
	{
		for (Infantry* infa : infantryGroup[i])
		{
			if (infa->died)
			{
				break;
			}
			int x = infa->positionNow.x;
			int y = infa->positionNow.y;
			Characters[x][y] -= 700;
			int qi = 2;
			Characters[x - 1][y] += qi;
			Characters[x + 1][y] += qi;
			Characters[x][y + 1] += qi;
			Characters[x][y - 1] += qi;
			Characters[x + 1][y + 1]++;
			Characters[x + 1][y - 1]++;
			Characters[x - 1][y + 1]++;
			Characters[x - 1][y - 1]++;
			x = infa->positionTarget.x;
			y = infa->positionTarget.y;
			Characters[x][y] -= 700;
			Characters[x][y] -= 700;
			Characters[x - 1][y] += qi;
			Characters[x + 1][y] += qi;
			Characters[x][y + 1] += qi;
			Characters[x][y - 1] += qi;
			Characters[x + 1][y + 1]++;
			Characters[x + 1][y - 1]++;
			Characters[x - 1][y + 1]++;
			Characters[x - 1][y - 1]++;
		}
		for (Dog* dog : dogGroup[i])
		{
			if (dog->died)
			{
				break;
			}
			int x = dog->positionNow.x;
			int y = dog->positionNow.y;
			Characters[x][y] -= 700;
			int qi = 2;
			Characters[x - 1][y] += qi;
			Characters[x + 1][y] += qi;
			Characters[x][y + 1] += qi;
			Characters[x][y - 1] += qi;
			Characters[x + 1][y + 1]++;
			Characters[x + 1][y - 1]++;
			Characters[x - 1][y + 1]++;
			Characters[x - 1][y - 1]++;
			x = dog->positionTarget.x;
			y = dog->positionTarget.y;
			Characters[x][y] -= 700;
			Characters[x][y] -= 700;
			Characters[x - 1][y] += qi;
			Characters[x + 1][y] += qi;
			Characters[x][y + 1] += qi;
			Characters[x][y - 1] += qi;
			Characters[x + 1][y + 1]++;
			Characters[x + 1][y - 1]++;
			Characters[x - 1][y + 1]++;
			Characters[x - 1][y - 1]++;
		}
		for (Tank* tank : tankGroup[i])
		{
			if (tank->died)
			{
				break;
			}
			int x = tank->positionNow.x;
			int y = tank->positionNow.y;
			Characters[x][y] -= 700;
			int qi = 2;
			Characters[x - 1][y] += qi;
			Characters[x + 1][y] += qi;
			Characters[x][y + 1] += qi;
			Characters[x][y - 1] += qi;
			Characters[x + 1][y + 1]++;
			Characters[x + 1][y - 1]++;
			Characters[x - 1][y + 1]++;
			Characters[x - 1][y - 1]++;
			x = tank->positionTarget.x;
			y = tank->positionTarget.y;
			Characters[x][y] -= 700;
			Characters[x][y] -= 700;
			Characters[x - 1][y] += qi;
			Characters[x + 1][y] += qi;
			Characters[x][y + 1] += qi;
			Characters[x][y - 1] += qi;
			Characters[x + 1][y + 1]++;
			Characters[x + 1][y - 1]++;
			Characters[x - 1][y + 1]++;
			Characters[x - 1][y - 1]++;
		}
	}


}

void Character::updateMove(float di) {
	if (died)
	{
		return;
	}
	if (!this)
	{
		return;
	}

	if (!this->getParent())
	{
		return;
	}
	if (repeat >= 30)
	{
		this->stop = 1;
		repeat = 0;
		//this->setGoal(positionNow);
	}
	if (stop)
	{
		return;
	}
	//this->setMapDestination(positionGoal);

	this->setZOrder(positionNow.y);
	Point TMposition = convertToTiledMap(this->getPosition());
	if (!this->numberOfRunningActions()) {
		positionNow = TMposition;
	}

	int x_goal = positionGoal.x;
	int y_goal = positionGoal.y;
	if (TMposition != positionGoal)
	{
		if (!this->numberOfRunningActions())
		{
			for (int i = 0; i < 118; i++)
			{
				for (int j = 0; j < 138; j++)
				{
					MapCondition[i][j] = MapInfo[i][j] + Block[i][j] + Buildings[i][j] + Characters[i][j] + this->MapDestination[i][j];
				}
			}
			Point BestTarget = positionNow;
			int x = positionNow.x;
			int y = positionNow.y;
			int direction = -1;
			int best = MapCondition[x][y];
			for (int i = 0; i < 8; i++)
			{
				if (Vec2(x + DIRECTION[i][0], y + DIRECTION[i][1]) == positionGoal)
				{
					//log("stop");

					if (MapCondition[x + DIRECTION[i][0]][y + DIRECTION[i][1]] < 0)
					{
						this->setGoal(positionNow);
						//positionGoal = positionNow;
						//log("stop");
						direction = -1;
						return;
						break;

					}
				}
				if (MapCondition[x + DIRECTION[i][0]][y + DIRECTION[i][1]] > best)
				{
					best = MapCondition[x + DIRECTION[i][0]][y + DIRECTION[i][1]];
					direction = i;

				}
			}
			if (formerValue > best)
			{
				repeat++;
			}
			formerValue = best;
			if (direction == -1)
			{
				return;
				/*repeat++;
				if (repeat == 1)
				{
					positionGoal = positionNow;
					repeat = 0;
				}*/
			}
			else
			{
				bool competely = 1;
				for (int i = 0; i < 8; i++)
				{

					if (Characters[x_goal + DIRECTION[i][0]][y_goal + DIRECTION[i][1]] > -200 &&
						Buildings[x_goal + DIRECTION[i][0]][y_goal + DIRECTION[i][1]] > -200 &&
						Block[x_goal + DIRECTION[i][0]][y_goal + DIRECTION[i][1]] > -200)
					{
						competely = 0;
						break;
					}
				}
				if (competely)
				{
					this->setGoal(positionNow);
					//positionGoal = positionNow;
				}
				BestTarget.x += DIRECTION[direction][0];
				BestTarget.y += DIRECTION[direction][1];
				int xx = BestTarget.x;
				int yy = BestTarget.y;
				bool neighbor = 0;
				Characters[xx][yy] -= 700;
				positionTarget = BestTarget;
				MoveTo* move = MoveTo::create(0.2f, convertFromTMToWorld(BestTarget));
				this->runAction(move);
				MapDestination[x][y] = -700;
			}
			/*bool competely = 1;
			for (int i = 0; i < 8; i++)
			{

				if (Characters[x_goal + DIRECTION[i][0]][y_goal + DIRECTION[i][1]] > -200 && Buildings[x_goal + DIRECTION[i][0]][y_goal + DIRECTION[i][1]] > -200)
				{
					competely = 0;
					break;
				}
			}
			if (competely)
			{
				this->setGoal(positionNow);
				//positionGoal = positionNow;
			}*/
		}
	}
}

void Character::updateAttack(float di)
{
	if (died)
	{
		return;
	}
	if (!attackTag)
	{
		stop = 0;
		return;
	}
	for (int s = (this->team + 1) % 2; s < 4; s += 2)
	{
		for (Infantry* enemy : infantryGroup[s])
		{
			if (enemy->getTag() == this->attackTag)
			{
				if (enemy->died)
				{
					this->attackTag = 0;
					return;
				}
				if (abs(this->positionNow.x - enemy->positionNow.x) > attackDistance + 1 || abs(this->positionNow.y - enemy->positionNow.y) > attackDistance + 1)
				{
					//this->positionGoal = enemy_infa->positionNow;
					if (this->positionGoal != enemy->positionNow)
					{
						this->setGoal(enemy->positionNow);
					}
				}
				else
				{
					this->attackInterval = this->attackInterval % 5;
					if (this->attackInterval == 0)
					{
						log("atk");
						this->setGoal(positionNow);
						stop = 1;
						enemy->beAttacked(this->attack);
					}
					this->attackInterval++;

				}
			}
		}
		for (Tank* enemy : tankGroup[s])
		{
			if (enemy->getTag() == this->attackTag)
			{
				if (enemy->died)
				{
					this->attackTag = 0;
					return;
				}
				if (abs(this->positionNow.x - enemy->positionNow.x) > attackDistance + 1 || abs(this->positionNow.y - enemy->positionNow.y) > attackDistance + 1)
				{
					//this->positionGoal = enemy_infa->positionNow;
					if (this->positionGoal != enemy->positionNow)
					{
						this->setGoal(enemy->positionNow);
					}
				}
				else
				{
					this->attackInterval = this->attackInterval % 5;
					if (this->attackInterval == 0)
					{
						log("atk");
						this->setGoal(positionNow);
						stop = 1;
						enemy->beAttacked(this->attack);
					}
					this->attackInterval++;

				}
			}
		}
		for (Dog* enemy : dogGroup[s])
		{
			if (enemy->getTag() == this->attackTag)
			{
				if (enemy->died)
				{
					this->attackTag = 0;
					return;
				}
				if (abs(this->positionNow.x - enemy->positionNow.x) > attackDistance + 1 || abs(this->positionNow.y - enemy->positionNow.y) > attackDistance + 1)
				{
					//this->positionGoal = enemy_infa->positionNow;
					if (this->positionGoal != enemy->positionNow)
					{
						this->setGoal(enemy->positionNow);
					}
				}
				else
				{
					this->attackInterval = this->attackInterval % 5;
					if (this->attackInterval == 0)
					{
						log("atk");
						this->setGoal(positionNow);
						stop = 1;
						enemy->beAttacked(this->attack);
					}
					this->attackInterval++;

				}
			}
		}
		for (Barrack* enemy : barrackGroup[s])
		{
			if (enemy->getTag() == this->attackTag)
			{
				if (enemy->died)
				{
					this->attackTag = 0;
					return;
				}
				if (abs(this->positionNow.x - enemy->positionCurrentTM.x) > attackDistance + 3 || abs(this->positionNow.y - enemy->positionCurrentTM.y) > attackDistance + 3)
				{
					//this->positionGoal = enemy_infa->positionNow;
					if (this->positionGoal != enemy->positionCurrentTM)
					{
						this->setGoal(enemy->positionCurrentTM);
					}
				}
				else
				{
					this->attackInterval = this->attackInterval % 5;
					if (this->attackInterval == 0)
					{
						log("atk");
						this->setGoal(positionNow);
						stop = 1;
						enemy->beAttacked(this->attack);
					}
					this->attackInterval++;

				}
			}
		}
		for (Warfactory* enemy : warfactoryGroup[s])
		{
			if (enemy->getTag() == this->attackTag)
			{
				if (enemy->died)
				{
					this->attackTag = 0;
					return;
				}
				if (abs(this->positionNow.x - enemy->positionCurrentTM.x) > attackDistance + 3 || abs(this->positionNow.y - enemy->positionCurrentTM.y) > attackDistance + 3)
				{
					//this->positionGoal = enemy_infa->positionNow;
					if (this->positionGoal != enemy->positionCurrentTM)
					{
						this->setGoal(enemy->positionCurrentTM);
					}
				}
				else
				{
					this->attackInterval = this->attackInterval % 5;
					if (this->attackInterval == 0)
					{
						log("atk");
						this->setGoal(positionNow);
						stop = 1;
						enemy->beAttacked(this->attack);
					}
					this->attackInterval++;

				}
			}
			if (enemy->getTag() == this->attackTag)
			{
				if (enemy->died)
				{
					this->attackTag = 0;
					return;
				}
				if (abs(this->positionNow.x - enemy->positionCurrentTM.x) > attackDistance + 3 || abs(this->positionNow.y - enemy->positionCurrentTM.y) > attackDistance + 3)
				{
					//this->positionGoal = enemy_infa->positionNow;
					if (this->positionGoal != enemy->positionCurrentTM)
					{
						this->setGoal(enemy->positionCurrentTM);
					}
				}
				else
				{
					this->attackInterval = this->attackInterval % 5;
					if (this->attackInterval == 0)
					{
						log("atk");
						this->setGoal(positionNow);
						stop = 1;
						enemy->beAttacked(this->attack);
					}
					this->attackInterval++;

				}
			}
		}
		for (Powerplant* enemy : powerplantGroup[s])
		{
			if (enemy->getTag() == this->attackTag)
			{
				if (enemy->died)
				{
					this->attackTag = 0;
					return;
				}
				if (abs(this->positionNow.x - enemy->positionCurrentTM.x) > attackDistance + 3 || abs(this->positionNow.y - enemy->positionCurrentTM.y) > attackDistance + 3)
				{
					//this->positionGoal = enemy_infa->positionNow;
					if (this->positionGoal != enemy->positionCurrentTM)
					{
						this->setGoal(enemy->positionCurrentTM);
					}
				}
				else
				{
					this->attackInterval = this->attackInterval % 5;
					if (this->attackInterval == 0)
					{
						log("atk");
						this->setGoal(positionNow);
						stop = 1;
						enemy->beAttacked(this->attack);
					}
					this->attackInterval++;

				}
			}
			if (enemy->getTag() == this->attackTag)
			{
				if (enemy->died)
				{
					this->attackTag = 0;
					return;
				}
				if (abs(this->positionNow.x - enemy->positionCurrentTM.x) > attackDistance + 3 || abs(this->positionNow.y - enemy->positionCurrentTM.y) > attackDistance + 3)
				{
					//this->positionGoal = enemy_infa->positionNow;
					if (this->positionGoal != enemy->positionCurrentTM)
					{
						this->setGoal(enemy->positionCurrentTM);
					}
				}
				else
				{
					this->attackInterval = this->attackInterval % 5;
					if (this->attackInterval == 0)
					{
						log("atk");
						this->setGoal(positionNow);
						stop = 1;
						enemy->beAttacked(this->attack);
					}
					this->attackInterval++;

				}
			}
		}
		for (Basement* enemy : basementGroup[s])
		{
			if (enemy->getTag() == this->attackTag)
			{
				if (enemy->died)
				{
					this->attackTag = 0;
					return;
				}
				if (abs(this->positionNow.x - enemy->positionCurrentTM.x) > attackDistance + 3 || abs(this->positionNow.y - enemy->positionCurrentTM.y) > attackDistance + 3)
				{
					//this->positionGoal = enemy_infa->positionNow;
					if (this->positionGoal != enemy->positionCurrentTM)
					{
						this->setGoal(enemy->positionCurrentTM);
					}
				}
				else
				{
					this->attackInterval = this->attackInterval % 5;
					if (this->attackInterval == 0)
					{
						log("atk");
						this->setGoal(positionNow);
						stop = 1;
						enemy->beAttacked(this->attack);
					}
					this->attackInterval++;

				}
			}
			if (enemy->getTag() == this->attackTag)
			{
				if (enemy->died)
				{
					this->attackTag = 0;
					return;
				}
				if (abs(this->positionNow.x - enemy->positionCurrentTM.x) > attackDistance + 3 || abs(this->positionNow.y - enemy->positionCurrentTM.y) > attackDistance + 3)
				{
					//this->positionGoal = enemy_infa->positionNow;
					if (this->positionGoal != enemy->positionCurrentTM)
					{
						this->setGoal(enemy->positionCurrentTM);
					}
				}
				else
				{
					this->attackInterval = this->attackInterval % 5;
					if (this->attackInterval == 0)
					{
						log("atk");
						this->setGoal(positionNow);
						stop = 1;
						enemy->beAttacked(this->attack);
					}
					this->attackInterval++;

				}
			}
		}
		for (Minefield* enemy : minefieldGroup[s])
		{
			if (enemy->getTag() == this->attackTag)
			{
				if (enemy->died)
				{
					this->attackTag = 0;
					return;
				}
				if (abs(this->positionNow.x - enemy->positionCurrentTM.x) > attackDistance + 3 || abs(this->positionNow.y - enemy->positionCurrentTM.y) > attackDistance + 3)
				{
					//this->positionGoal = enemy_infa->positionNow;
					if (this->positionGoal != enemy->positionCurrentTM)
					{
						this->setGoal(enemy->positionCurrentTM);
					}
				}
				else
				{
					this->attackInterval = this->attackInterval % 5;
					if (this->attackInterval == 0)
					{
						log("atk");
						this->setGoal(positionNow);
						stop = 1;
						enemy->beAttacked(this->attack);
					}
					this->attackInterval++;

				}
			}
			if (enemy->getTag() == this->attackTag)
			{
				if (enemy->died)
				{
					this->attackTag = 0;
					return;
				}
				if (abs(this->positionNow.x - enemy->positionCurrentTM.x) > attackDistance + 3 || abs(this->positionNow.y - enemy->positionCurrentTM.y) > attackDistance + 3)
				{
					//this->positionGoal = enemy_infa->positionNow;
					if (this->positionGoal != enemy->positionCurrentTM)
					{
						this->setGoal(enemy->positionCurrentTM);
					}
				}
				else
				{
					this->attackInterval = this->attackInterval % 5;
					if (this->attackInterval == 0)
					{
						log("atk");
						this->setGoal(positionNow);
						stop = 1;
						enemy->beAttacked(this->attack);
					}
					this->attackInterval++;

				}
			}
		}
	}
}

void Game::updateZOrder(float di)
{
	for (int i = 0; i < 4; i++)
	{
		for (auto infa : infantryGroup[i])
		{
			if(!infa->died)
			infa->setZOrder(infa->positionNow.y);
		}
		for (auto dog : dogGroup[i])
		{
			if(!dog->died)
			dog->setZOrder(dog->positionNow.y);
		}
		for (auto tank : tankGroup[i])
		{
			if(!tank->died)
			tank->setZOrder(tank->positionNow.y);
		}
	}
}

Vec2 Safe(Vec2 &position)
{
	Vec2 safe = position;
	if (safe.x == 0)
	{
		safe.x = 1;
	}
	if (safe.x == 118)
	{
		safe.x = 117;
	}
	if (safe.y == 0)
	{
		safe.y = 1;
	}
	if (safe.y == 138)
	{
		safe.y = 137;
	}
	return safe;
}

void Game::numberClientEvent(cocos2d::network::SIOClient *client, const std::string& data)
{
	log("Client Called");
	log("%s", data.c_str());
	int number = atoi(data.c_str());
	UserDefault::getInstance()->setIntegerForKey(PLAYER_NUMBER, number);
	return;
}

void Game::onConnect(cocos2d::network::SIOClient *client)
{
	return;
}

void Game::onMessage(cocos2d::network::SIOClient *client, const std::string& data)
{
	return;
}


void Game::onClose(cocos2d::network::SIOClient *client)
{
	return;
}

void Game::onError(cocos2d::network::SIOClient *client, const std::string& data)
{
	return;
}

