#include "StartScene.h"
#include "SimpleAudioEngine.h"
#include "HelloWorldScene.h"

USING_NS_CC;

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

	MenuItemFont *itemreturn = MenuItemFont::create("Return", CC_CALLBACK_1(Start::menureturnCallback, this));

	Menu *mn = Menu::create(itemreturn, NULL);

	this->addChild(mn);

	return true;
}

void Start::menureturnCallback(Ref *pSender)
{
	auto sc = HelloWorld::createScene();
	Director::getInstance()->replaceScene(sc);
}