#include"AboutScene.h"
#include"SimpleAudioEngine.h"

USING_NS_CC;

Scene* AboutScene::createScene()
{
	return AboutScene::create();
}

static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

bool AboutScene::init()
{
	if (!Scene::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//label
	auto label = Label::createWithSystemFont("wait to add", "Arial", 60);
	label->setPosition(Vec2(origin.x + visibleSize.width / 3, origin.y + visibleSize.height * 2 / 3));
	this->addChild(label);

	//menu
	MenuItemImage *returnitem = MenuItemImage::create(
		"menu/return.png",
		"menu/return.png",
		CC_CALLBACK_1(AboutScene::returnCallback, this));
	returnitem->setScale(0.5);

	Menu *mn = Menu::create(returnitem, nullptr);
	mn->setPosition(Vec2(origin.x + visibleSize.width*2/3, origin.y + visibleSize.height / 3));
	this->addChild(mn);

	return true;
}

void AboutScene::returnCallback(Ref* pSender)
{
	Director::getInstance()->popScene();
}