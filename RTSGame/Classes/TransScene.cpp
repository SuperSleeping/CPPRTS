#include"TransScene.h"
#include"SimpleAudioEngine.h"
#include"GameScene.h"

USING_NS_CC;

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
	this->addChild(mn);

	return true;
}

void TransScene::newCallback(Ref* pSender)
{
	
}

void TransScene::joinCallback(Ref* pSender)
{
	
}

void TransScene::returnCallback(Ref* pSender)
{
	Director::getInstance()->popScene();
}