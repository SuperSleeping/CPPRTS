#include "Game.h"
#include "HelloWorldScene.h"
#include <iostream>

#include"GameElement/Infantry.h"
#include"GameElement/Basement.h"

vector<Infantry> infantry;

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
	
	//添加游戏层(?)
	//////#############################################################################!!!!!!!!解决一下回调函数监听器什么的捆绑在哪个node上的问题
	auto menuLayer = Layer::create();
	this->addChild(menuLayer);

	//置入地图
	map = TMXTiledMap::create("map/map1.tmx");
	this->addChild(map,-1);

	_grass = map->getLayer("grass");
	_grasswet = map->getLayer("grasswet");
	_ground = map->getLayer("ground");
	_meta = map->getLayer("meta");

	//尺寸、坐标信息初始化
	visibleSize.x = 1600;
	visibleSize.y = 900;
	mapSize.x = map->getMapSize().width;
	mapSize.y = map->getMapSize().height;
	tmSize.x = map->getTileSize().width;
	tmSize.y = map->getTileSize().height;

	//菜单栏
	auto menu = Menu::create();

	auto basement = MenuItemImage::create("Game/button/basement_button1.png", "Game/button/basement_button3.png", CC_CALLBACK_1(Game::buttonBasement, this));
	auto basementx = MenuItemImage::create("Game/button/basement_button2.png", "Game/button/basement_button2.png", CC_CALLBACK_1(Game::buttonBasementx, this));
	basement->setAnchorPoint(Vec2(0, 0));
	basementx->setAnchorPoint(Vec2(0, 0));
	menu->addChild(basement, 1);
	menu->addChild(basementx, 2);

	menu->setPosition(Vec2(visibleSize.x-500,0));

	menuLayer->addChild(menu,1);
	



	//触发事件
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	//@鼠标移动到边界的时候屏幕移动
	auto mouseListener = EventListenerMouse::create();
	mouseListener->onMouseMove = CC_CALLBACK_1(Game::onMouseMove, this);

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

void Game::update(float dt)
{

}

/***************/
//Player events
//Constructions:
/***************/
void Game::onMouseMove(cocos2d::Event* event)
{
	{
		//视野移动
		EventMouse* e = (EventMouse*)event;
		Vec2 position = e->getLocationInView();
		Vec2 target = position - map->getPosition();
		Vec2 changetime = Vec2(1.2,1.6);
		if (position.x > visibleSize.x - 100)
		{
			if (!(map->numberOfRunningActions()))
			{
				auto map_move = MoveTo::create(changetime.x, Vec2(-(4000 - visibleSize.x), map->getPosition().y));
				map->runAction(map_move);
			}
		}
		else if (position.x < 100)
		{
			if (!(map->numberOfRunningActions()))
			{
				auto map_move = MoveTo::create(changetime.x, Vec2(0, map->getPosition().y));
				map->runAction(map_move);
			}
		}
		else if (position.y < 100)
		{
			if (!(map->numberOfRunningActions()))
			{
				auto map_move = MoveTo::create(changetime.y, Vec2(map->getPosition().x, 0));
				map->runAction(map_move);
			}
		}
		else if (position.y > visibleSize.y - 100)
		{
			if (!(map->numberOfRunningActions()))
			{
				auto map_move = MoveTo::create(changetime.y, Vec2(map->getPosition().x, -(4000 - visibleSize.y)));
				map->runAction(map_move);
			}
		}
		else
		{
			map->stopAllActions();
		}
		
		//建筑状态
		/*
		if (building)
		{
			auto factory = Sprite::create("bd.png");
			if (!virtual_factory.empty()) {
				for (auto fac : virtual_factory) {
					fac->removeFromParent();
				}
				virtual_factory.popBack();
			}
			virtual_factory.pushBack(factory);
			for (auto fac : virtual_factory) {
				map->addChild(fac, 3);
				fac->setOpacity(100);
				fac->setPosition(target);
			}
		}
		*/
	};
}

/***************/
//菜单button
/***************/

void Game::buttonBasement(Ref* pSender)
{
	buildState = Building::BuildingType::BASEMENT;
	auto testspr = Sprite::create("Game/building/basement.png");
	Point position = Vec2(visibleSize.x / 2, visibleSize.y / 2);
	position = convertToTiledMap(position);
	testspr->setPosition(position);
	map->addChild(testspr,100);
}

void Game::buttonBasementx(Ref* pSender)
{
	log("Not enough resources or a wrong routine");
}

/***************/
//瓦片地图API
/***************/
bool Game::readBlock(Point position)
{
	position = convertToMapLayer(position);
	Point tmPoint = convertToTiledMap(position);
	int GID = _meta->getTileGIDAt(tmPoint);
	if (tmPoint!=NULL)
	{
		auto properties = map->getPropertiesForGID(GID).asValueMap();
		if (!properties.empty())
		{
			auto block = properties["Block"].asString();
			if (block == "True")return 1;
			else return 0;
		}
	}
	else return 0;
}

void Game::addBlock(Point position)
{
	position = convertToMapLayer(position);
	Point tmPoint = convertToTiledMap(position);
	int GID = _meta->getTileGIDAt(tmPoint);
	if (tmPoint != NULL)
	{
		auto properties = map->getPropertiesForGID(GID).asValueMap();
		if (!properties.empty())
		{
			auto block = properties["Block"].asString();
			block = "True";
		}
	}
}

void Game::removeBlock(Point position)
{
	position = convertToMapLayer(position);
	Point tmPoint = convertToTiledMap(position);
	int GID = _meta->getTileGIDAt(tmPoint);
	if (tmPoint != NULL)
	{
		auto properties = map->getPropertiesForGID(GID).asValueMap();
		if (!properties.empty())
		{
			auto block = properties["Block"].asString();
			block = "False";
		}
	}
}

void Game::menuReturn(cocos2d::Ref* pSender)
{
	
}