#include "Game.h"
#include "HelloWorldScene.h"
#include <iostream>

#include"GameElement/Infantry.h"


vector<Infantry> infantry;

Scene* Game::createScene()
{
	auto scene = Scene::create();

	auto settingLayer = Game::create();
	
	scene->addChild(settingLayer);

	return scene;
}

bool Game::init()
{
	if (!Scene::init())
	{
		return false;
	}
	visibleSize = Director::getInstance()->getVisibleSize();
	
	//置入地图
	map = TMXTiledMap::create("map/map.tmx");

	_grass = map->getLayer("grass");
	_grasswet = map->getLayer("grasswet");
	_ground = map->getLayer("ground");
	_sea = map->getLayer("sea");

	_meta = map->getLayer("meta");

	_meta->setVisible(false);

	
	this->addChild(map,-1);


	//触发事件
	auto dispatcher = Director::getInstance()->getEventDispatcher();













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

Point Game::convertToTiledMap(Point position)
{

}

/**************
//Player events
//Constructions:
***************/
void Game::mouseDown(cocos2d::Event* event)
{
	EventMouse *e = (EventMouse*)event;

}

bool Game::readBlock(Point position)
{
	position = convertToWorldSpace(position);
	Point tmPoint = convertToTiledMap(position);
	int GID = _meta->getTileGIDAt(tmPoint);
	if (tmPoint!=NULL)
	{
		auto properties = map->getPropertiesForGID(GID).asValueMap;
		if (!properties.empty())
		{
			auto block = properties["Block"].asString();
			if (block == "True")return 1;
			else return 0;
		}
	}
}

void Game::setBlock(Point position)
{

}


void Game::menuReturn(cocos2d::Ref* pSender)
{
	
}