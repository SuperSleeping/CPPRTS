#include "AboutUsScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Scene* AboutUs::createScene()
{
	return AboutUs::create();
}

static void problemLoading(const char* filename)
{
	printf("Error while loading:%s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

bool AboutUs::init()
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

	auto label1 = Label::createWithTTF("The Developer:", "fonts/Marker Felt.ttf", 30);
	label1->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
	this->addChild(label1);

	MenuItemFont *itemreturn = MenuItemFont::create("Return", CC_CALLBACK_1(AboutUs::menureturnCallback, this));
	itemreturn->setPosition(Vec2(origin.x + visibleSize.width / 3, origin.y + visibleSize.height / 3));

	Menu *mn = Menu::create(itemreturn, NULL);
	this->addChild(mn);

	return true;
}

void AboutUs::menureturnCallback(Ref *pSender)
{
	Director::getInstance()->popScene();
}