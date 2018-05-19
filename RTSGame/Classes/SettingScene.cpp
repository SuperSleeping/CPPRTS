#include "SettingScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Scene* Setting::createScene()
{
	return Setting::create();
}

static void problemLoading(const char* filename)
{
	printf("Error while loading:%s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

bool Setting::init()
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

	auto toggleMenuItemsound = MenuItemToggle::createWithCallback(CC_CALLBACK_1(Setting::menusoundCallback, this),
		MenuItemFont::create("On"), MenuItemFont::create("Off"), NULL);
	auto toggleMenuItemmusic = MenuItemToggle::createWithCallback(CC_CALLBACK_1(Setting::menumusicCallback, this),
		MenuItemFont::create("On"), MenuItemFont::create("Off"), NULL);
	MenuItemFont *itemok = MenuItemFont::create("OK", CC_CALLBACK_1(Setting::menuokCallback, this));
	MenuItemFont *itemreturn = MenuItemFont::create("Return", CC_CALLBACK_1(Setting::menureturnCallback, this));

	Menu* mn = Menu::create(toggleMenuItemmusic, toggleMenuItemsound, itemok, itemreturn,NULL);
	mn->alignItemsVertically();

	this->addChild(mn);

	return true;
}

void Setting::menuokCallback(Ref *pSender)
{
	Director::getInstance()->popScene();
}

void Setting::menureturnCallback(Ref *pSender)
{
	Director::getInstance()->popScene();
}

void Setting::menusoundCallback(Ref *pSender)
{}

void Setting::menumusicCallback(Ref *pSender)
{}