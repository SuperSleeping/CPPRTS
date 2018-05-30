#include"TemMap.h"
#include"SimpleAudioEngine.h"

USING_NS_CC;

Scene* temmap::createScene()
{
	return temmap::create();
}

static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

bool temmap::init()
{
	if (!Scene::init())
	{
		return false;
	}
	//left map
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();






	//right menu
	MenuItemImage *pauseitem = MenuItemImage::create(
		"menu/pause.png",
		"menu/pause.png",
		CC_CALLBACK_1(temmap::pauseCallback, this));

	MenuItemImage *tipitem = MenuItemImage::create(
		"menu/tip.png",
		"menu/tip.png",
		CC_CALLBACK_1(temmap::tipCallback, this));

	Menu *mn1 = Menu::create(pauseitem,tipitem,nullptr);
	mn1->alignItemsHorizontallyWithPadding(0);
	mn1->setPosition(Vec2(origin.x + visibleSize.width * 5/6, origin.y + visibleSize.height*7/8));
	this->addChild(mn1);

	//middle picture
	Sprite *midpic = Sprite::create("menu/midpic.jpg");
	midpic->setPosition(Vec2(visibleSize.width / 3, visibleSize.height / 2));
	this->addChild(midpic);









	return true;
}

void temmap::pauseCallback(Ref* pSender)
{
	
}

void temmap::tipCallback(Ref* pSender)
{

}
