#include"RootScene.h"
#include"GameScene.h"
#include"EndVerify.h"
#include"MySetting.h"
#include"StartScene.h"
#include"AboutScene.h"
#include"TransScene.h"

USING_NS_CC;
using namespace CocosDenshion;

Scene* RootScene::createScene()
{
	return RootScene::create();
}

static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

bool RootScene::init()
{
	if (!Scene::init())
	{
		return false;
	}
	log("init");

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	
	//set background scene  
	auto bg = Sprite::create("enteringScene/enterPic.jpg");
	bg->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));//这句自己根据需要修改  
	float bgx = bg->getTextureRect().getMaxX();
	float bgy = bg->getTextureRect().getMaxY();
	bg->setScaleX(visibleSize.width / bgx); //设置精灵宽度缩放比例  
	bg->setScaleY(visibleSize.height / bgy);
	this->addChild(bg);
	
	//menu
	MenuItemImage *gamebeganitem = MenuItemImage::create(
		"menu/gamebegan.png",
		"menu/gamebegan.png",
		CC_CALLBACK_1(RootScene::menugamebeganCallback, this));

	MenuItemImage *settingitem = MenuItemImage::create(
		"menu/setting1.png",
		"menu/setting1.png",
		CC_CALLBACK_1(RootScene::menusettingCallback, this));

	MenuItemImage *exititem = MenuItemImage::create(
		"menu/exit1.png",
		"menu/exit1.png",
		CC_CALLBACK_1(RootScene::menuexitCallback, this));

	MenuItemImage *aboutitem = MenuItemImage::create(
		"menu/about.png",
		"menu/about.png",
		CC_CALLBACK_1(RootScene::menuaboutCallback, this));

	gamebeganitem->setScale(0.5);
	settingitem->setScale(0.5);
	exititem->setScale(0.5);
	aboutitem->setScale(0.5);

	Menu *mn = Menu::create(gamebeganitem,settingitem,aboutitem,exititem,nullptr);
	mn->alignItemsVerticallyWithPadding(15);
	mn->setPosition(Vec2(origin.x+visibleSize.width*10/27,origin.y+visibleSize.height*3/8));
	this->addChild(mn);
	
	if (UserDefault::getInstance()->getBoolForKey(MUSIC_KEY,1))
	{
		SimpleAudioEngine::getInstance()->playBackgroundMusic("music/HM2.mp3", true);
		//检验背景音乐是否正在播放
		bool flag = &SimpleAudioEngine::isBackgroundMusicPlaying;
		log("%d", flag);
	}

	return true;
}

void RootScene::menugamebeganCallback(Ref* pSender)
{
	auto sc = TransScene::createScene();
	Director::getInstance()->pushScene(sc);
}

void RootScene::menusettingCallback(Ref* pSender)
{
	auto sc = MySetting::createScene();
	Director::getInstance()->pushScene(sc);
}

void RootScene::menuexitCallback(Ref* pSender)
{
	auto sc= EndVerify::createScene();
	Director::getInstance()->pushScene(sc);
}

void RootScene::menuaboutCallback(Ref* pSender)
{
	auto sc = AboutScene::createScene();
	Director::getInstance()->pushScene(sc);
}

//Scene生命周期函数
void RootScene::onEnter()
{
	Scene::onEnter();
	log("onEnter");
	return;
}

void RootScene::onEnterTransitionDidFinish()
{
	Scene::onEnterTransitionDidFinish();
	log("onEnterTransitionDidFinish");
	/*if (UserDefault::getInstance()->getBoolForKey(MUSIC_KEY))
	{
		SimpleAudioEngine::getInstance()->playBackgroundMusic("music/HM2.mp3", true);
		//检验背景音乐是否正在播放
		bool flag = &SimpleAudioEngine::isBackgroundMusicPlaying;
		log("%d", flag);
	}*/
	return;
}

void RootScene::onExit()
{
	Scene::onExit();
	log("onExit");
	return;
}

void RootScene::onExitTransitionDidStart()
{
	Scene::onExitTransitionDidStart();
	log("onExitTransitionDidStart");
	return;
}

void RootScene::cleanup()
{
	Scene::cleanup();
	log("cleanup");
	//SimpleAudioEngine::getInstance()->stopBackgroundMusic();
	return;
}