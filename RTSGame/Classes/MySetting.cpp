#include"MySetting.h"
#include <iostream>
#include "extensions/cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace CocosDenshion;

Scene* MySetting::createScene()
{
	return MySetting::create();
}

static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

bool MySetting::init()
{
	if (!Scene::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto bg = Sprite::create("menu/settingbg.png");
	bg->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(bg);

	//menu
	auto musicitem1 = MenuItemImage::create("menu/left.png", "menu/left.png");
	auto musicitem2 = MenuItemImage::create("menu/right.png", "menu/right.png");
	auto musicitem = MenuItemToggle::createWithCallback(CC_CALLBACK_1(MySetting::musicCallback, this),//函数指针
		musicitem1, musicitem2,//菜单项集合
		NULL//集合结束
	);

	auto sounditem1 = MenuItemImage::create("menu/left.png", "menu/left.png");
	auto sounditem2 = MenuItemImage::create("menu/right.png", "menu/right.png");
	auto sounditem = MenuItemToggle::createWithCallback(CC_CALLBACK_1(MySetting::soundCallback, this),//函数指针
		sounditem1, sounditem2,//菜单项集合
		NULL//集合结束
	);

	Menu *mn = Menu::create(musicitem, sounditem, nullptr);
	mn->alignItemsVerticallyWithPadding(70);
	mn->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height * 4 / 9));
	this->addChild(mn);

	MenuItemImage *returnitem = MenuItemImage::create(
		"menu/return.png",
		"menu/return.png",
		CC_CALLBACK_1(MySetting::returnCallback, this));

	MenuItemImage *saveitem = MenuItemImage::create(
		"menu/save.png",
		"menu/save.png",
		CC_CALLBACK_1(MySetting::saveCallback, this));

	returnitem->setScale(0.5);
	saveitem->setScale(0.5);
	Menu *mn1 = Menu::create(saveitem,returnitem, nullptr);
	mn1->alignItemsHorizontallyWithPadding(300);
	mn1->setPosition(Vec2(origin.x + visibleSize.width*13/27, origin.y + visibleSize.height*10/54));
	this->addChild(mn1);

	UserDefault *defaults = UserDefault::getInstance();
	//设置开关项状态
	//开关左边为开右边为关
	if (defaults->getBoolForKey(MUSIC_KEY,1))
	{
		musicitem->setSelectedIndex(0);
	}
	else
	{
		musicitem->setSelectedIndex(1);
	}
	if (defaults->getBoolForKey(SOUND_KEY,1))
	{
		sounditem->setSelectedIndex(0);
	}
	else
	{
		sounditem->setSelectedIndex(1);
	}

	std::string username = defaults->getStringForKey(USER_NAME, "User");
	auto nameSetting = EditBox::create(Size(500, 70), Scale9Sprite::create("menu/cin.png"));
	nameSetting->setPosition(Vec2(origin.x + visibleSize.width * 19 / 36, origin.y + visibleSize.height * 12 / 16));
	nameSetting->setPlaceHolder(username.c_str());
	nameSetting->setFontColor(Color3B::BLACK);
	nameSetting->setTag(123);
	this->addChild(nameSetting);

	//try
	auto tem = Sprite::create("tem/br.png");
	tem->setPosition(Vec2(1400, 150));
	this->addChild(tem);
	auto eventListener = EventListenerKeyboard::create();
	eventListener->onKeyPressed = [](EventKeyboard::KeyCode keyCode, Event* event) {
		auto loc = event->getCurrentTarget()->getPosition();
		switch (keyCode) {
		case EventKeyboard::KeyCode::KEY_ENTER:
		{
			loc.x += 50;
			loc.y += 50;
			event->getCurrentTarget()->setPosition(loc.x, loc.y);
		}
			break;
		}
	};
	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(eventListener,tem);

	return true;
}

void MySetting::returnCallback(Ref* pSender)
{
	Director::getInstance()->popScene();
}

void MySetting::saveCallback(Ref* pSender)
{
	auto name = reinterpret_cast<EditBox*>(this->getChildByTag(123));
	std::string newname = name->getText();
	UserDefault::getInstance()->setStringForKey(USER_NAME, newname);
	Director::getInstance()->popScene();
}

void MySetting::musicCallback(Ref* pSender)
{
	UserDefault *defaults = UserDefault::getInstance();
	if (defaults->getBoolForKey(MUSIC_KEY,1))
	{
		defaults->setBoolForKey(MUSIC_KEY, false);
		SimpleAudioEngine::getInstance()->stopBackgroundMusic();
	}
	else
	{
		defaults->setBoolForKey(MUSIC_KEY, true);
		SimpleAudioEngine::getInstance()->playBackgroundMusic("music/HM2.mp3", true);
	}
}

void MySetting::soundCallback(Ref* pSender)
{
	UserDefault *defaults = UserDefault::getInstance();
	if (defaults->getBoolForKey(SOUND_KEY, 1))
	{
		defaults->setBoolForKey(SOUND_KEY, false);
	}
	else
	{
		defaults->setBoolForKey(SOUND_KEY, true);
		//可选播放下音效
	}
}
