#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "Game.h"
#include "Setting.h"
#include "Developer.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

void HelloWorld::update(float dt)
{
	auto picture = this->getChildByTag(1);
	picture->setPosition(picture->getPosition() + Vec2(2, -2));
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


    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2.menu create
	//create 3 lables
	//each lable-type menu item relates to a new scene.

	auto labelNewgame = Label::createWithTTF("New Game", "fonts/consola.ttf", 30);

	auto labelSetting = Label::createWithTTF("Settings", "fonts/consola.ttf", 30);

	auto labelDeveloper = Label::createWithTTF("Developers", "fonts/consola.ttf", 30);

	//put lables into menu

	auto newgameItem = MenuItemLabel::create(
										labelNewgame,
										CC_CALLBACK_1(HelloWorld::menuIntoLabelNewgame, this));

	auto settingItem = MenuItemLabel::create(
										labelSetting,
										CC_CALLBACK_1(HelloWorld::menuIntoLabelSetting, this));

	auto developerItem = MenuItemLabel::create(
										labelDeveloper,
										CC_CALLBACK_1(HelloWorld::menuIntoLabelDeveloper, this));

	float menuPositionX = visibleSize.width / 2 + origin.x;
	float menuPositionY = visibleSize.height / 3 + origin.y;

	newgameItem->setPosition(Vec2(menuPositionX, menuPositionY));
	settingItem->setPosition(Vec2(menuPositionX, menuPositionY - settingItem->getContentSize().height - 10));
	developerItem->setPosition(Vec2(menuPositionX, menuPositionY - 2*developerItem->getContentSize().height - 20));

	//add a menu item with "X" image, which is clicked to quit the program
	//    you may modify it.

	// add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));

    if (closeItem == nullptr ||
        closeItem->getContentSize().width <= 0 ||
        closeItem->getContentSize().height <= 0)
    {
        problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
    }
    else
    {
        float x = origin.x + visibleSize.width - closeItem->getContentSize().width/2;
        float y = origin.y + closeItem->getContentSize().height/2;
        closeItem->setPosition(Vec2(x,y));
    }

    // create menu, it's an autorelease object
    auto menu = Menu::create();
 
	menu->addChild(closeItem, 1);
	menu->addChild(newgameItem, 1);
	menu->addChild(settingItem, 1);
	menu->addChild(developerItem, 1);

	menu->setPosition(Vec2::ZERO);	
	this->addChild(menu, 1);


    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label

	/*
    auto label = Label::createWithTTF("Welcome", "fonts/consola.ttf", 50);
    if (label == nullptr)
    {
        problemLoading("'fonts/consola.ttf'");
    }
    else
    {
        // position the label on the center of the screen
        label->setPosition(Vec2(origin.x + visibleSize.width/2,
                                origin.y + visibleSize.height - label->getContentSize().height));

        // add the label as a child to this layer
        this->addChild(label, 1);
    }
	*/

    // add "HelloWorld" splash screen"
    auto sprite = Sprite::create("enteringScene/enterPic.jpg");
    if (sprite == nullptr)
    {
        problemLoading("'enterPic.jpg'");
    }
    else
    {
        // position the sprite on the center of the screen
        sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/3*2 - 20 + origin.y));

		sprite->setTag(1);

        // add the sprite as a child to this layer
        this->addChild(sprite, 0);
    }

	this->scheduleUpdate();

    return true;
}

void HelloWorld::menuIntoLabelNewgame(cocos2d::Ref* pSender)
{
	auto GameScene = Game::createScene();

	auto transToGame = TransitionFade::create(0.5f, GameScene);

	Director::getInstance()->pushScene(transToGame);
}

void HelloWorld::menuIntoLabelSetting(cocos2d::Ref* pSender)
{
	auto SettingScene = Setting::createScene();

	auto transToSetting = TransitionFade::create(0.5f, SettingScene);

	Director::getInstance()->pushScene(transToSetting);
}

void HelloWorld::menuIntoLabelDeveloper(cocos2d::Ref* pSender)
{
	auto DeveloperScene = Developer::createScene();

	auto transToDeveloper = TransitionFade::create(0.5f, DeveloperScene);

	Director::getInstance()->pushScene(transToDeveloper);
}



void HelloWorld::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() and exit(0) as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}
