#include "Game.h"
#include "HelloWorldScene.h"

USING_NS_CC;

Scene* Game::createScene()
{
	return Game::create();
}

bool Game::init()
{
	if (!Scene::init())
	{
		return false;
	}











	return true;
}

void Game::sceneLoad()
{

}



void Game::mapInit()
{
	//using list view to choose map.
}

void Game::menuReturn(cocos2d::Ref* pSender)
{

}
