#include "Game.h"
#include "HelloWorldScene.h"

#include <iostream>

Scene* Game::createScene()
{
	auto scene = Scene::create();

	auto settingLayer = Game::create();
	
	scene->addChild(settingLayer);

	return scene;
}

void Game::update(float dt)
{

	
}

bool Game::init()
{
	if (!Scene::init())
	{
		return false;
	}
	visibleSize = Director::getInstance()->getVisibleSize();


	/*
	mapChoice();
	mapInit(mapType);
	*/
	mapInit(1);

	//GameScene上的鼠标监听、事件分配
	auto mouseListener = EventListenerMouse::create();
	mouseListener->onMouseDown = CC_CALLBACK_1(Game::mouseDown, this);
	dispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);



	return true;
}


void Game::menuReturn(cocos2d::Ref* pSender)
{

}

//map init
/*
void Game::mapChoice()
{
	auto Map1 = MenuItemImage::create("Game/mapChoice/button1.png", "Game/mapChoice/button1.png", CC_CALLBACK_1(Game::setMapType1, this));
	auto Map2 = MenuItemImage::create("Game/mapChoice/button2.png", "Game/mapChoice/button2.png", CC_CALLBACK_1(Game::setMapType2, this));
	Map1->setPositionX(-50);
	Map2->setPositionX(50);

	auto menu = Menu::create(Map1, Map2, nullptr);

	menu->setPosition(visibleSize.x/2,visibleSize.y/2);
	this->addChild(menu);
}

void Game::setMapType1(cocos2d::Ref* pSender)
{
	mapType = 1;
}

void Game::setMapType2(cocos2d::Ref* pSender)
{
	mapType = 2;
}
*/
void Game::mapInit(int type)
{
	//Map setting init
	if (type == 1)
		map = TMXTiledMap::create("test.tmx");
	else
		map = TMXTiledMap::create("maptest.tmx");

	map->setPosition(visibleSize.x / 2, visibleSize.y / 2);
	this->addChild(map);

	_background = map->getLayer("background");
	_object = map->getLayer("object");
	_meta = map->getLayer("meta");

	_meta->setVisible(false);


	//Object init//

	//Object
	auto object = GameElement::create("character.png", Vec2(200, 200));

	character.push_back(object);
	this->addChild(object->picture);

	/*/////////////////////////////////////////////////////////
	//Player for test
	player = Sprite::create("character.png");
	this->addChild(player);

	auto objectgroup = map->getObjectGroup("object");
	auto spawnpoint = objectgroup->getObject("object");
	Point playerPoint;
	
	playerPoint.x = spawnpoint.at("x").asFloat();
	playerPoint.y = spawnpoint.at("y").asFloat();

	player->setAnchorPoint(Point(0, 0));
	player->setPosition(playerPoint);
	*//////////////////////////////////////////////////////////

	dispatcher = Director::getInstance()->getEventDispatcher();


	this->scheduleUpdate();
}

/**************
//player events
//moving constructions:
//@up-W @donw-S @left-A @right-D
***************/
void Game::mouseDown(cocos2d::Event* event)
{
	EventMouse *e = (EventMouse*)event;
	character[0]->positionGoal = e->getLocation();

	//人物移动
	character[0]->move();
}


