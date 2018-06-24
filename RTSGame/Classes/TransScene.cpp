#include"TransScene.h"
#include"SimpleAudioEngine.h"
#include"Game.h"
#include "WaitingScene.h"
#include "UDP.h"
#include <stdlib.h>
#include <Windows.h>
#include <direct.h>


USING_NS_CC;
using namespace CocosDenshion;

Scene* TransScene::createScene()
{
	return TransScene::create();
}

static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

bool TransScene::init()
{
	if (!Scene::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto pic = Sprite::create("menu/addpic.png");
	pic->setPosition(Vec2(origin.x + visibleSize.width / 3, origin.y + visibleSize.height / 2));
	this->addChild(pic);
	//menu
	MenuItemImage *newitem = MenuItemImage::create(
		"menu/new.png",
		"menu/new.png",
		CC_CALLBACK_1(TransScene::newCallback, this));

	MenuItemImage *joinitem = MenuItemImage::create(
		"menu/join.png",
		"menu/join.png",
		CC_CALLBACK_1(TransScene::joinCallback, this));

	MenuItemImage *returnitem = MenuItemImage::create(
		"menu/return.png",
		"menu/return.png",
		CC_CALLBACK_1(TransScene::returnCallback, this));

	newitem->setScale(0.5);
	joinitem->setScale(0.5);
	returnitem->setScale(0.5);
	Menu *mn = Menu::create(newitem,joinitem,returnitem,nullptr);
	mn->alignItemsVerticallyWithPadding(25);
	mn->setPosition(Vec2(origin.x + visibleSize.width*2/3, origin.y + visibleSize.height / 2));
	mn->setTag(123);
	this->addChild(mn);

	MenuItemImage *twoItem = MenuItemImage::create("menu/2P.png", "menu/2P.png", CC_CALLBACK_1(TransScene::twoCallback, this));
	MenuItemImage *fourItem = MenuItemImage::create("menu/4P.png", "menu/4P.png", CC_CALLBACK_1(TransScene::fourCallback, this));
	MenuItemImage *backItem = MenuItemImage::create("menu/return.png", "menu/return.png", CC_CALLBACK_1(TransScene::backCallback, this));
	twoItem->setScale(0.5);
	fourItem->setScale(0.5);
	backItem->setScale(0.5);
	Menu *mm = Menu::create(twoItem, fourItem, backItem, nullptr);
	mm->alignItemsVerticallyWithPadding(25);
	mm->setPosition(Vec2(origin.x + visibleSize.width * 2 / 3, origin.y + visibleSize.height / 2));
	mm->setVisible(false);
	mm->setTag(321);
	this->addChild(mm);

	//music
	if (UserDefault::getInstance()->getBoolForKey(MUSIC_KEY))
	{
		SimpleAudioEngine::getInstance()->playBackgroundMusic("music/Ready The Army.mp3", true);
		//检验背景音乐是否正在播放
		bool flag = &SimpleAudioEngine::isBackgroundMusicPlaying;
		log("%d", flag);
	}

	return true;
}

void TransScene::newCallback(Ref* pSender)
{
	/*将参数2改为SW_HIDE即可隐藏运行窗口 暂时用于查看服务端输出*/
	WinExec("order.bat", SW_NORMAL);
	char hostIp[30];
	getHostIp(hostIp);
	strcat(hostIp, ":3000");
	log("%s", hostIp);
	UserDefault::getInstance()->setStringForKey(HOST_IP, hostIp);
	auto mm = this->getChildByTag(321);
	auto mn = this->getChildByTag(123);
	mn->setVisible(false);
	mm->setVisible(true);
	/*int number = 2;								//加入游戏的玩家人数。允许玩家设置为2或4
	UserDefault::getInstance()->setIntegerForKey(PLAYERS, number);
	serverOperation(number);
	auto sc = Game::createScene();
	Director::getInstance()->replaceScene(sc);*/
	return;
}

void TransScene::joinCallback(Ref* pSender)
{
	char hostIp[30];
	clientOperation(hostIp);
	strcat(hostIp, ":3000");
	log("%s",hostIp);
	UserDefault::getInstance()->setStringForKey(HOST_IP, hostIp);
	auto sc = WaitingScene::createScene();
	Director::getInstance()->replaceScene(sc);
	return;
}

void TransScene::returnCallback(Ref* pSender)
{
	Director::getInstance()->popScene();
}

void TransScene::backCallback(Ref *pSender)
{
	auto mn = this->getChildByTag(123);
	auto mm = this->getChildByTag(321);
	mm->setVisible(false);
	mn->setVisible(true);
	return;
}

void TransScene::twoCallback(Ref* pSender)
{
	UserDefault::getInstance()->setIntegerForKey(PLAYERS, 2);
	std::thread server(serverOperation, 2);
	server.detach();
	//serverOperation(2);
	auto sc = WaitingScene::createScene();
	Director::getInstance()->replaceScene(sc);
	return;
}

void TransScene::fourCallback(Ref *pSender)
{
	UserDefault::getInstance()->setIntegerForKey(PLAYERS, 4);
	std::thread server(serverOperation, 2);
	server.detach();
	//serverOperation(4);
	auto sc = WaitingScene::createScene();
	Director::getInstance()->replaceScene(sc);
	return;
}