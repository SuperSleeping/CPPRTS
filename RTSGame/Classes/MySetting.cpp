#include"MySetting.h"
#include"SimpleAudioEngine.h"

USING_NS_CC;

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
	auto label1 = Label::createWithSystemFont("Difficulty", "Arial", 45);
	label1->setPosition(Vec2(origin.x + visibleSize.width / 4, origin.y + visibleSize.height*13/16));
	this->addChild(label1);

	auto label2 = Label::createWithSystemFont("Speed", "Arial", 45);
	label2->setPosition(Vec2(origin.x + visibleSize.width / 4, origin.y + visibleSize.height *17/32));
	this->addChild(label2);

	auto label3 = Label::createWithSystemFont("Sound", "Arial", 45);
	label3->setPosition(Vec2(origin.x + visibleSize.width / 4, origin.y + visibleSize.height *9/32));
	this->addChild(label3);

	//menu
	auto difficultyitem1= MenuItemImage::create("menu/left.png", "menu/left.png");
	auto difficultyitem2 = MenuItemImage::create("menu/right.png", "menu/right.png");
	auto difficultyitem = MenuItemToggle::createWithCallback(CC_CALLBACK_1(MySetting::difficultyCallback, this),//函数指针
		difficultyitem1, difficultyitem2,//菜单项集合
		NULL//集合结束
	);

	auto speeditem1 = MenuItemImage::create("menu/left.png", "menu/left.png");
	auto speeditem2 = MenuItemImage::create("menu/right.png", "menu/right.png");
	auto speeditem = MenuItemToggle::createWithCallback(CC_CALLBACK_1(MySetting::speedCallback, this),//函数指针
		speeditem1, speeditem2,//菜单项集合
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

	Menu *mn = Menu::create(difficultyitem,speeditem,sounditem,backtorootitem,nullptr);
	mn->alignItemsVerticallyWithPadding(45);
	mn->setPosition(Vec2(origin.x + visibleSize.width*2/3, origin.y + visibleSize.height / 2));
	this->addChild(mn);

	return true;
}

void MySetting::backtorootCallback(Ref* pSender)
{
	Director::getInstance()->popScene();
}

void MySetting::difficultyCallback(Ref* pSender)
{
	
}

void MySetting::speedCallback(Ref* pSender)
{
	
}

void MySetting::soundCallback(Ref* pSender)
{
	
}
