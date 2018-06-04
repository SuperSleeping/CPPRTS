#include "Developer.h"
#include "HelloWorldScene.h"

USING_NS_CC;

Scene* Developer::createScene()
{
	return Developer::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

bool Developer::init()
{
	if (!Scene::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	////////////////////////////
	//2.create
	//input background

	auto picture = Sprite::create("enteringScene/background1.jpg");

	picture->setContentSize(Size(visibleSize.width, visibleSize.height));
	picture->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));

	this->addChild(picture, 0);

	//input menu

	auto labelReturn = Label::createWithTTF("Return", "fonts/consola.ttf", 30);

	auto returnItem = MenuItemLabel::create(
		labelReturn,
		CC_CALLBACK_1(Developer::menuReturn, this));

	returnItem->setPosition(Vec2(visibleSize.width / 2 + 200, visibleSize.height / 3 * 2 - labelReturn->getContentSize().height - 20));

	returnItem->setColor(Color3B(1, 1, 1));

	auto menu = Menu::create();

	menu->addChild(returnItem, 1);

	menu->setPosition(Vec2::ZERO);

	this->addChild(menu);

	return true;
	
}

void Developer::menuReturn(cocos2d::Ref* pSender)
{
	auto HelloWorldScene = HelloWorld::createScene();

	auto transToHelloWorld = TransitionFade::create(0.5f, HelloWorldScene);

	Director::getInstance()->pushScene(transToHelloWorld);
}