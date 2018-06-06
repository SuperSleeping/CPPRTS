#include "Game.h"
#include "HelloWorldScene.h"



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





	return true;
}


void Game::menuReturn(cocos2d::Ref* pSender)
{

}

//map init
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

void Game::mapInit(int type)
{
	if (type == 1)
		map = TMXTiledMap::create("test.tmx");
	else
		map = TMXTiledMap::create("maptest.tmx");

//	map->setPosition(visibleSize.x / 2, visibleSize.y / 2);
	this->addChild(map);

	_background = map->getLayer("background");
	_object = map->getLayer("object");
	_meta = map->getLayer("meta");

	_meta->setVisible(false);

	player = Sprite::create("character.png");
	this->addChild(player);

	auto objectgroup = map->getObjectGroup("object");
	auto spawnpoint = objectgroup->getObject("object");
	Point playerPoint;
	
	playerPoint.x = spawnpoint.at("x").asFloat();
	playerPoint.y = spawnpoint.at("y").asFloat();

	player->setAnchorPoint(Point(0, 0));
	player->setPosition(playerPoint);

	dispatcher = Director::getInstance()->getEventDispatcher();

	auto keyboardListener = EventListenerKeyboard::create();
	keyboardListener->onKeyPressed = CC_CALLBACK_2(Game::keyPressed, this);

	dispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);

//	this->schedule(schedule_selector(Game::update), 0.1);
	this->scheduleUpdate();
}

/**************
//player events
//moving constructions:
//@up-W @donw-S @left-A @right-D
***************/
void Game::keyPressed(EventKeyboard::KeyCode keycode, cocos2d::Event *event)
{
	auto curPosition = player->getPosition();

	if (keycode == EventKeyboard::KeyCode::KEY_W)
	{
		player->setPositionY(curPosition.y + 5);
	}
	else if (keycode == EventKeyboard::KeyCode::KEY_A)
	{
		player->setPositionX(curPosition.x - 5);
	}
	else if (keycode == EventKeyboard::KeyCode::KEY_S)
	{
		player->setPositionY(curPosition.y - 5);
	}
	else if (keycode == EventKeyboard::KeyCode::KEY_D)
	{
		player->setPositionX(curPosition.x + 5);
	}
}

/*
void Game::touchReW()
{

}

void Game::touchOnA()
{

}

void Game::touchReA()
{

}

void Game::touchOnS()
{

}

void Game::touchReS()
{

}

void Game::touchOnD()
{

}
void Game::touchReD()
{

}

*/

