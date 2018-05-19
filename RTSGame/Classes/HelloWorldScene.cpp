#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "SettingScene.h"
#include "StartScene.h"
#include "AboutUsScene.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

	//2.background
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	Sprite *background = Sprite::create("background.jpg");
	background->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
	this->addChild(background);

	//3.menu
	MenuItemFont::setFontName("Times New Roman");
	MenuItemFont::setFontSize(30);
	MenuItemFont *start = MenuItemFont::create("Start", CC_CALLBACK_1(HelloWorld::menustartCallback, this));
	MenuItemFont *setting = MenuItemFont::create("Setting", CC_CALLBACK_1(HelloWorld::menusettingCallback, this));
	MenuItemFont *aboutus = MenuItemFont::create("About us", CC_CALLBACK_1(HelloWorld::menuaboutusCallback, this));

	Menu* mn = Menu::create(start, setting, aboutus, NULL);
	mn->alignItemsVertically();

	this->addChild(mn);

    return true;
}


void HelloWorld::menustartCallback(Ref *pSender)
{
	auto sc = Start::createScene();
	Director::getInstance()->replaceScene(sc);
}

void HelloWorld::menusettingCallback(Ref *pSender)
{
	auto sc = Setting::createScene();
	Director::getInstance()->pushScene(sc);
}

void HelloWorld::menuaboutusCallback(Ref *pSender)
{
	auto sc = AboutUs::createScene();
	Director::getInstance()->pushScene(sc);
}