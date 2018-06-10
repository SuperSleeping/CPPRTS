#include"RootScene.h"
#include"SimpleAudioEngine.h"
#include"GameScene.h"
#include"EndVerify.h"
#include"MySetting.h"

USING_NS_CC;

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

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//left picture
	Sprite *leftbg = Sprite::create("enteringScene/enterPic.jpg");
	leftbg->setPosition(Vec2(visibleSize.width / 3, visibleSize.height / 2));
	this->addChild(leftbg);

	//menu
	MenuItemImage *singleitem = MenuItemImage::create(
		"menu/single.png",
		"menu/single.png",
		CC_CALLBACK_1(RootScene::menusinmemchoiceCallback, this));

	MenuItemImage *doubleitem = MenuItemImage::create(
		"menu/double.png",
		"menu/double.png",
		CC_CALLBACK_1(RootScene::menumulmemchoiceCallback, this));

	MenuItemImage *settingitem = MenuItemImage::create(
		"menu/setting.png",
		"menu/setting.png",
		CC_CALLBACK_1(RootScene::menusettingCallback, this));

	MenuItemImage *makeritem = MenuItemImage::create(
		"menu/maker.png",
		"menu/maker.png",
		CC_CALLBACK_1(RootScene::menumakerCallback, this));

	MenuItemImage *enditem = MenuItemImage::create(
		"menu/exit.png",
		"menu/exit.png",
		CC_CALLBACK_1(RootScene::menuendCallback, this));

	MenuItemFont::setFontSize(33);
	Menu *mn = Menu::create(singleitem,doubleitem,settingitem,makeritem,enditem,nullptr);
	mn->alignItemsVerticallyWithPadding(30);
	mn->setPosition(Vec2(origin.x+visibleSize.width*3/4,origin.y+visibleSize.height/2));
	this->addChild(mn);

	return true;
}

void RootScene::menusinmemchoiceCallback(Ref* pSender)
{
	auto sc = GameScene::createScene();
	Director::getInstance()->replaceScene(sc);
}

void RootScene::menumulmemchoiceCallback(Ref* pSender)
{

}

void RootScene::menusettingCallback(Ref* pSender)
{
	auto sc = MySetting::createScene();
	Director::getInstance()->pushScene(sc);
}

void RootScene::menumakerCallback(Ref* pSender)
{

}

void RootScene::menuendCallback(Ref* pSender)
{
	auto sc= EndVerify::createScene();
	Director::getInstance()->pushScene(sc);
}
