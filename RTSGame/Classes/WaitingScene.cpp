#include "WaitingScene.h"
#include "Game.h"       //要进入的游戏场景

USING_NS_CC;

Scene* WaitingScene::createScene()
{
	return WaitingScene::create();
}

static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

bool WaitingScene::init()
{
	if (!Scene::init())
	{
		return false;
	}
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	std::string hostIp = UserDefault::getInstance()->getStringForKey(HOST_IP);
	sioClient = cocos2d::network::SocketIO::connect(hostIp, *this);

	auto label = Label::create("Waiting for other players...", "Arial", 36);
	label->setColor(Color3B::WHITE);
	label->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
	this->addChild(label);

	return true;
}

void WaitingScene::onConnect(cocos2d::network::SIOClient *client)
{
	return;
}

void WaitingScene::onMessage(cocos2d::network::SIOClient *client, const std::string& data)
{
	int players = UserDefault::getInstance()->getIntegerForKey(PLAYERS);
	std::string message = data;
	message.erase(0, 1);
	message.pop_back();
	if (message.length() == 1)
	{
		if (players - 1 == atoi(message.c_str()))
		{
			sioClient->emit("beginServerEvent", "begin");
		}
	}
	else
	{
		if (strcmp(message.c_str(), "begin") == 0)
		{
			/*切换进入下一个场景*/
			auto sc = Game::create();
			Director::getInstance()->replaceScene(sc);
		}
	}
	return;
}

void WaitingScene::onClose(cocos2d::network::SIOClient *client)
{
	return;
}

void WaitingScene::onError(cocos2d::network::SIOClient *client, const std::string& data)
{
	return;
}
