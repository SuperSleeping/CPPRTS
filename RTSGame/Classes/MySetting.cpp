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

	//label
	auto label1 = Label::createWithSystemFont("Name", "Arial", 45);
	label1->setPosition(Vec2(origin.x + visibleSize.width / 4, origin.y + visibleSize.height*13/16));
	this->addChild(label1);

	auto label2 = Label::createWithSystemFont("Music", "Arial", 45);
	label2->setPosition(Vec2(origin.x + visibleSize.width / 4, origin.y + visibleSize.height *17/32));
	this->addChild(label2);

	auto label3 = Label::createWithSystemFont("Sound", "Arial", 45);
	label3->setPosition(Vec2(origin.x + visibleSize.width / 4, origin.y + visibleSize.height *9/32));
	this->addChild(label3);

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

	MenuItemImage *backtorootitem = MenuItemImage::create(
		"menu/backtoroot.png",
		"menu/backtoroot.png",
		CC_CALLBACK_1(MySetting::backtorootCallback, this));

	//保存设置项暂时用了返回的图片来做菜单
	MenuItemImage *saveitem = MenuItemImage::create(
		"menu/backtoroot.png",
		"menu/backtoroot.png",
		CC_CALLBACK_1(MySetting::saveCallback, this));
	//下面那个返回改成保存
	Menu *mn = Menu::create(musicitem,sounditem,backtorootitem,saveitem,nullptr);
	mn->alignItemsVerticallyWithPadding(45);
	mn->setPosition(Vec2(origin.x + visibleSize.width*2/3, origin.y + visibleSize.height / 2));
	this->addChild(mn);

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
	auto nameSetting = EditBox::create(Size(500, 70), Scale9Sprite::create("editbox.jpg"));
	nameSetting->setAnchorPoint(Point(0, 0));
	nameSetting->setPosition(Vec2(0,0));
	nameSetting->setPlaceHolder(username.c_str());
	nameSetting->setFontColor(Color3B::BLACK);
	nameSetting->setTag(123);
	this->addChild(nameSetting);

	return true;
}

void MySetting::backtorootCallback(Ref* pSender)
{
	Director::getInstance()->popScene();
}

void MySetting::saveCallback(Ref* pSender)
{
	auto name = reinterpret_cast<EditBox*>(this->getChildByTag(123));
	std::string newname = name->getText();
	if (newname.length())
	{
		UserDefault::getInstance()->setStringForKey(USER_NAME, newname);
	}
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
