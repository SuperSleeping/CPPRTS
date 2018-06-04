#include"EndVerify.h"
#include"SimpleAudioEngine.h"

USING_NS_CC;

Scene* EndVerify::createScene()
{
	return EndVerify::create();
}

static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

bool EndVerify::init()
{
	if (!Scene::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//label
	auto label = Label::createWithSystemFont("Sure to exit?","Arial",60);
	label->setPosition(Vec2(origin.x + visibleSize.width/3, origin.y + visibleSize.height*2/3));
	this->addChild(label);

	//menu
	MenuItemImage *verifyitem = MenuItemImage::create(
		"menu/yes.png",
		"menu/yes.png",
		CC_CALLBACK_1(EndVerify::verifyCallback, this));

	MenuItemImage *cancelitem = MenuItemImage::create(
		"menu/cancel.png",
		"menu/cancel.png",
		CC_CALLBACK_1(EndVerify::cancelCallback, this));

	Menu *mn = Menu::create(verifyitem,cancelitem,nullptr);
	mn->alignItemsHorizontallyWithPadding(200);
	mn->setPosition(Vec2(origin.x + visibleSize.width/2, origin.y + visibleSize.height/3));
	this->addChild(mn);

	return true;
}

void EndVerify::verifyCallback(Ref* pSender)
{
	Director::getInstance()->end();
}

void EndVerify::cancelCallback(Ref* pSender)
{
	Director::getInstance()->popScene();
}