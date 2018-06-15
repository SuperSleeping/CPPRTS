#include "Game.h"
#include "HelloWorldScene.h"
#include <iostream>

#include"GameElement/Basement.h"
#include"GameElement/Barrack.h"
#include"GameElement/Warfactory.h"
#include"GameElement/Minefield.h"
#include"GameElement/Powerplant.h"

#include"GameElement/Infantry.h"

//建立vector储存gameElement
//4个数组分别存放四个国家的财产
vector<Infantry*> infantryGroup[4];

vector<Basement*> basementGroup[4];
vector<Barrack*> barrackGroup[4];
vector<Warfactory*> warfactoryGroup[4];
vector<Minefield*> minefieldGroup[4];
vector<Powerplant*> powerplantGroup[4];


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

	
	//菜单栏
	{
	menu = Menu::create();
	//按钮的z坐标决定图像和触发函数 z坐标较大的图像显示在上层 但是先调用的是z坐标较小的函数
	//解决方法很粗暴 是改变了按钮的图案
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
	dispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);









	/*
	//GameScene上的鼠标监听、事件分配
	auto mouseListener = EventListenerMouse::create();
	mouseListener->onMouseDown = CC_CALLBACK_1(Game::mouseDown, this);
	dispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);
	*/

	this->scheduleUpdate();

	return true;
}

//更新函数
void Game::update(float dt)
{
	
}

void Game::menuUpdate()
{
	//检测是否为某建筑的选中状态，再根据相应功能检测资源改变菜单
	if (selectedState == NULL)
	{

	}
	else if (selectedState == Building::BuildingType::BASEMENT)
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
	Vec2 position = e->getLocationInView();
	Vec2 target = convertToMapLayer(position);

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
		target = convertToNeightborTiledMap(target);
		OKtobuilt = 1 - readOccupiedTile(target, buildState);

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
		Point possiblePosition = convertFromTMToWorld(target);
		BuildingPictureWithMouse->setPosition(possiblePosition);
		BuildingPictureWithMouse->setOpacity(150);
		map->addChild(BuildingPictureWithMouse, 100);
	}
}

void Game::onMouseDown(cocos2d::Event* event)
{
	EventMouse* e = (EventMouse*)event;
	enum {screen,world,tiledmapTM, tiledmapW};
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
			basementGroup[myTeam].push_back(building);
			game->addChild(building, position[tiledmapTM].y);
		}
		else if (buildState == Building::BuildingType::BARRACK)
		{
			auto building = Barrack::create(position[tiledmapW]);
			barrackGroup[myTeam].push_back(building);
			game->addChild(building, position[tiledmapTM].y);
		}
		else if (buildState == Building::BuildingType::MINEFIELD)
		{
			auto building = Minefield::create(position[tiledmapW]);
			minefieldGroup[myTeam].push_back(building);
			game->addChild(building, position[tiledmapTM].y);
		}
		else if (buildState == Building::BuildingType::POWERPLANT)
		{
			auto building = Powerplant::create(position[tiledmapW]);
			powerplantGroup[myTeam].push_back(building);
			game->addChild(building, position[tiledmapTM].y);
		}
		else if (buildState == Building::BuildingType::WARFACTORY)
		{
			auto building = Warfactory::create(position[tiledmapW]);
			warfactoryGroup[myTeam].push_back(building);
			game->addChild(building, position[tiledmapTM].y);
		}
		else
		{
			log("Wrong : buildState not found!");
		}

		//添加建筑占地 改变地图属性
		changeOccupiedTile(position[tiledmapTM],buildState);

		//退出建筑状态
		buildState = NULL;
		map->removeChild(BuildingPictureWithMouse);
	}


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
	buildState = Character::CharacterType::INFANTRY;
}

void Game::buttonDog(Ref* pSender)
{
	buildState = Character::CharacterType::DOG;
}

void Game::buttonTank(Ref* pSender)
{
	buildState = Character::CharacterType::TANK;
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