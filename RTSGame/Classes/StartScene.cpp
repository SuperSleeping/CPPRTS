#include "StartScene.h"
#include "SimpleAudioEngine.h"
#include "HelloWorldScene.h"
#include "UDP.h"

USING_NS_CC;
using namespace cocos2d::network;

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
	Sprite *background = Sprite::create("background.jpg");
	background->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
	this->addChild(background);

	MenuItemFont *itemnew = MenuItemFont::create("New Game", CC_CALLBACK_1(Start::menunewCallback, this));
	MenuItemFont *itemjoin = MenuItemFont::create("Join", CC_CALLBACK_1(Start::menujoinCallback, this));
	MenuItemFont *itemrefresh = MenuItemFont::create("Refresh", CC_CALLBACK_1(Start::menurefreshCallback, this));
	MenuItemFont *itemreturn = MenuItemFont::create("Return", CC_CALLBACK_1(Start::menureturnCallback, this));
	MenuItemFont *itembegin = MenuItemFont::create("Begin", CC_CALLBACK_1(Start::menubeginCallback, this));

	Menu *mn = Menu::create(itemnew, itemjoin, itemrefresh, itemreturn, itembegin, NULL);
	mn->alignItemsVertically();

	this->addChild(mn);

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
	auto sc = HelloWorld::createScene();
	Director::getInstance()->replaceScene(sc);
}
void Start::menujoinCallback(Ref *pSender)
{
	char hostIp[30];
	clientOperation(hostIp);
	strcat(hostIp, ":3000");
	sioClient = SocketIO::connect(hostIp, *this);
	return;
}
void Start::menunewCallback(Ref *pSender)
{
	serverOperation();
	return;
}
void Start::menurefreshCallback(Ref *pSender)
{
	return;
	/*
	构建一个房间列表
	*/
}



//委托协议方法
void Start::onConnect(SIOClient *client)
{

}

void Start::onClose(SIOClient *client)
{

}

void Start::onMessage(SIOClient *client, const std::string& data)
{

}

void Start::onError(SIOClient *client, const std::string& data)
{

}