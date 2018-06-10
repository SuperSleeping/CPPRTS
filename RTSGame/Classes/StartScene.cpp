#include "StartScene.h"
#include "SimpleAudioEngine.h"
#include "UDP.h"
#include "GameScene.h"
#include <stdlib.h>
#include <Windows.h>
#include <direct.h>

USING_NS_CC;
using namespace CocosDenshion;

Scene* Start::createScene()
{
	return Start::create();
}

static void problemLoading(const char* filename)
{
	printf("Error while loading:%s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

bool Start::init()
{
	if (!Scene::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	MenuItemFont *itemnew = MenuItemFont::create("New Game", CC_CALLBACK_1(Start::menunewCallback, this));
	MenuItemFont *itemjoin = MenuItemFont::create("Join", CC_CALLBACK_1(Start::menujoinCallback, this));
	MenuItemFont *itemrefresh = MenuItemFont::create("Refresh", CC_CALLBACK_1(Start::menurefreshCallback, this));
	MenuItemFont *itemreturn = MenuItemFont::create("Return", CC_CALLBACK_1(Start::menureturnCallback, this));
	MenuItemFont *itembegin = MenuItemFont::create("Begin", CC_CALLBACK_1(Start::menubeginCallback, this));

	Menu *mn = Menu::create(itemnew, itemjoin, itemrefresh, itemreturn, itembegin, NULL);
	mn->alignItemsVertically();

	this->addChild(mn);

	if (UserDefault::getInstance()->getBoolForKey(MUSIC_KEY))
	{
		SimpleAudioEngine::getInstance()->playBackgroundMusic("music/Ready The Army.mp3", true);
		//检验背景音乐是否正在播放
		bool flag = &SimpleAudioEngine::isBackgroundMusicPlaying;
		log("%d", flag);
	}

	return true;
}

void Start::menubeginCallback(Ref *pSender)
{
	return;
	/*
	加载gamescene场景 目前尚未定义
	auto sc = GameScene:create();
	Director::getInstance()->replaceScene(sc);
	*/
}
void Start::menureturnCallback(Ref *pSender)
{
	/*auto sc = HelloWorld::createScene();
	Director::getInstance()->replaceScene(sc);*/
}
void Start::menujoinCallback(Ref *pSender)
{
	char hostIp[30];
	clientOperation(hostIp);
	strcat(hostIp, ":3000");
	UserDefault::getInstance()->setStringForKey(HOST_IP, hostIp);
	auto sc = GameScene::createScene();
	Director::getInstance()->replaceScene(sc);
	return;
}
void Start::menunewCallback(Ref *pSender)
{
	char filePath[500] = { 0 };
	getcwd(filePath, 500);
	for (int i = 0; i < 50; i++)
	{
		log("%d", filePath[i]);
	}
	log("%s", filePath);
	char path[520] = "set path=\"";
	strcat(path, filePath);
	strcat(path, "\\nodejs\"");
	//WinExec(path, SW_NORMAL);
	log("%s", path);
	/*将参数2改为SW_HIDE即可隐藏运行窗口 暂时用于查看服务端输出*/
	char server[520] = "node ";
	strcat(server, filePath);
	strcat(server, "\\nodejs\\app.js");
	strcat(path, "&&");
	strcat(path, server);
	system(path);
	//WinExec(path, SW_NORMAL);
	log("%s", path);
	char hostIp[30];
	getHostIp(hostIp);
	strcat(hostIp, ":3000");
	log("%s", hostIp);
	UserDefault::getInstance()->setStringForKey(HOST_IP, hostIp);
	serverOperation();
	auto sc = GameScene::createScene();
	Director::getInstance()->replaceScene(sc);
	return;
}
void Start::menurefreshCallback(Ref *pSender)
{
	return;
	/*
	构建一个房间列表
	*/
}
